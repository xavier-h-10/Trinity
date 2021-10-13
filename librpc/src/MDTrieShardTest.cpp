#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "MDTrieShardClient.h"
#include "trie.h"
#include <tqdm.h>
#include <future>
#include <atomic>
#include <tuple>
#include <iostream>
#include <fstream>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

const int DIMENSION = 6; 
n_leaves_t n_lines = 14252681;
const int BATCH_SIZE = 128;
std::atomic<int> active_thread_num {0};
std::atomic<int> finished_thread_num {0};

std::atomic<TimeStamp> latency_start = 0;
std::atomic<TimeStamp> latency_diff = 0;

vector<vector <int32_t>> *get_data_vector(){

/** 
    Get data from the OSM dataset stored in a vector
*/

  FILE *fp = fopen("../libmdtrie/bench/data/osm_combined_updated.csv", "r");
  char *line = nullptr;
  size_t len = 0;
  ssize_t read;

  if (fp == nullptr)
  {
      fprintf(stderr, "file not found\n");
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof(cwd)) != nullptr) {
          printf("Current working dir: %s\n", cwd);
      } else {
          perror("getcwd() error");
      }
      exit(EXIT_FAILURE);
  }  

  tqdm bar;
  n_leaves_t n_points = 0;
  auto data_vector = new vector<vector <int32_t>>;
  while ((read = getline(&line, &len, fp)) != -1)
  {
      bar.progress(n_points, n_lines);
      vector<int32_t> point;
      char *token = strtok(line, ",");
      char *ptr;
      for (dimension_t i = 0; i < 8; i++){

          if (i == 1){
              token = strtok(nullptr, ",");
              token = strtok(nullptr, ",");
          }
          token = strtok(nullptr, ",");
          if (i < 8 - DIMENSION)
              continue;

          point.push_back(strtoul(token, &ptr, 10));
      }
      data_vector->push_back(point);
      n_points ++;
      // if (n_points == n_lines / 2)
      //   break;
  }  
  bar.finish();
  return data_vector;
}


std::tuple<uint32_t, uint32_t, uint32_t> insert_each_client(vector<vector <int32_t>> *data_vector, int client_number, int client_index){

  auto client = MDTrieClient();
  uint32_t start_pos = data_vector->size() / client_number * client_index;
  uint32_t end_pos = data_vector->size() / client_number * (client_index + 1) - 1;

  if (client_index == client_number - 1)
    end_pos = data_vector->size() - 1;

  uint32_t total_points_to_insert = end_pos - start_pos + 1;
  uint32_t warmup_cooldown_points = total_points_to_insert / 3;

  int sent_count = 0;
  uint32_t current_pos;

  TimeStamp start = 0; 
  TimeStamp diff = 0;

  for (current_pos = start_pos; current_pos <= end_pos; current_pos++){
    
    if (current_pos == start_pos + warmup_cooldown_points){
      start = GetTimestamp();
    }

    if (sent_count != 0 && sent_count % BATCH_SIZE == 0){
        for (uint32_t j = current_pos - sent_count; j < current_pos; j++){
            client.insert_rec(j);
            if (j == end_pos - warmup_cooldown_points){

              diff = GetTimestamp() - start;
            }
        }
        sent_count = 0;
    }

    vector<int32_t> data_point = (*data_vector)[current_pos];
    client.insert_send(data_point, current_pos);
    sent_count ++;
  }

  for (uint32_t j = end_pos - sent_count + 1; j <= end_pos; j++){
      client.insert_rec(j);
      if (j == end_pos - warmup_cooldown_points){

        diff = GetTimestamp() - start;
      }
  }
  return std::make_tuple(((float) (total_points_to_insert - 2 * warmup_cooldown_points) / diff) * 1000000, diff, total_points_to_insert - 2 * warmup_cooldown_points);
}

std::tuple<uint32_t, float> total_client_insert(vector<vector <int32_t>> *data_vector, int client_number){

  std::vector<std::future<std::tuple<uint32_t, uint32_t, uint32_t>>> threads; 
  threads.reserve(client_number);

  for (int i = 0; i < client_number; i++){

    threads.push_back(std::async(insert_each_client, data_vector, client_number, i));
  }  

  uint32_t total_throughput = 0;
  uint32_t total_latency = 0;
  uint32_t total_points = 0;
  for (int i = 0; i < client_number; i++){

    std::tuple<uint32_t, uint32_t, uint32_t> return_tuple = threads[i].get();
    total_throughput += std::get<0>(return_tuple);
    total_latency += std::get<1>(return_tuple);    
    total_points += std::get<2>(return_tuple);

    if (i == 0)
      cout << "total throughput: [" << total_throughput << "] total_latency: [" << total_latency << "] total_points: [" << total_points << "]" << endl;
  } 

  return std::make_tuple(total_throughput, (float) total_latency / total_points);  
}


// LOOKUP Code

std::tuple<uint32_t, uint32_t, uint32_t> lookup_each_client(vector<vector <int32_t>> *data_vector, int client_number, int client_index){

  auto client = MDTrieClient();
  uint32_t start_pos = data_vector->size() / client_number * client_index;
  uint32_t end_pos = data_vector->size() / client_number * (client_index + 1) - 1;

  if (client_index == client_number - 1)
    end_pos = data_vector->size() - 1;

  uint32_t total_points_to_lookup = end_pos - start_pos + 1;
  uint32_t warmup_cooldown_points = total_points_to_lookup / 3;

  int sent_count = 0;
  uint32_t current_pos;

  TimeStamp start = 0; 
  TimeStamp diff = 0;

  for (current_pos = start_pos; current_pos <= end_pos; current_pos++){
    
    if (current_pos == start_pos + warmup_cooldown_points){

      // cout << "started measurement: " << client_index << endl;
      start = GetTimestamp();
    }

    if (sent_count != 0 && sent_count % BATCH_SIZE == 0){
        for (uint32_t j = current_pos - sent_count; j < current_pos; j++){
            std::vector<int32_t> rec_vect;
            client.primary_key_lookup_rec(rec_vect, j);
            if (j == end_pos - warmup_cooldown_points){
              diff = GetTimestamp() - start;
            }
        }
        sent_count = 0;
    }

    vector<int32_t> data_point = (*data_vector)[current_pos];
    client.primary_key_lookup_send(current_pos);
    sent_count ++;
  }

  // cout << "ended measurement: " << client_index << endl;

  for (uint32_t j = end_pos - sent_count + 1; j <= end_pos; j++){
      std::vector<int32_t> rec_vect;
      if (j == end_pos - warmup_cooldown_points){
        diff = GetTimestamp() - start;
      }
      client.primary_key_lookup_rec(rec_vect, j);
  }

  return std::make_tuple(((float) (total_points_to_lookup - 2 * warmup_cooldown_points) / diff) * 1000000, diff, total_points_to_lookup - 2 * warmup_cooldown_points);
}

std::tuple<uint32_t, float> total_client_lookup(vector<vector <int32_t>> *data_vector, int client_number){

  std::vector<std::future<std::tuple<uint32_t, uint32_t, uint32_t>>> threads; 
  threads.reserve(client_number);

  for (int i = 0; i < client_number; i++){

    threads.push_back(std::async(lookup_each_client, data_vector, client_number, i));
  }  

  uint32_t total_throughput = 0;
  uint32_t total_latency = 0;
  uint32_t total_points = 0;
  for (int i = 0; i < client_number; i++){

    std::tuple<uint32_t, uint32_t, uint32_t> return_tuple = threads[i].get();
    total_throughput += std::get<0>(return_tuple);
    total_latency += std::get<1>(return_tuple);    
    total_points += std::get<2>(return_tuple);

    // if (i == 0)
    //   cout << "total throughput: [" << total_throughput << "] total_latency: [" << total_latency << "] total_points: [" << total_points << "]" << endl;
  } 

  return std::make_tuple(total_throughput, (float) total_latency / total_points);  
}

std::tuple<uint32_t, uint32_t, uint32_t> range_search_each_client(vector<vector <int32_t>> *start_range_collection, vector<vector <int32_t>> *end_range_collection, int client_number, int client_index){

  auto client = MDTrieClient();
  uint32_t start_pos = start_range_collection->size() / client_number * client_index;
  uint32_t end_pos = start_range_collection->size() / client_number * (client_index + 1) - 1;

  if (client_index == client_number - 1)
    end_pos = start_range_collection->size() - 1;

  uint32_t total_points_to_lookup = end_pos - start_pos + 1;
  uint32_t warmup_cooldown_points = total_points_to_lookup / 7;
  uint32_t current_pos;
  int sent_count = 0;

  TimeStamp start = 0; 
  TimeStamp diff = 0;
  uint32_t total_points_returned = 0;

  for (current_pos = start_pos; current_pos <= end_pos; current_pos++){
    
    if (current_pos == start_pos + warmup_cooldown_points){
      start = GetTimestamp();
    }

    if (sent_count != 0 && sent_count % BATCH_SIZE == 0){
        for (uint32_t j = current_pos - sent_count; j < current_pos; j++){

          std::vector<int32_t> rec_vect;

          // TimeStamp start_here = GetTimestamp();
          client.range_search_trie_rec(rec_vect);
          // cout << "receive request time time: " << GetTimestamp() - start_here << " for index: " << j << " with points: " << rec_vect.size() << endl;

          if (j == end_pos - warmup_cooldown_points){
            diff = GetTimestamp() - start;
          }

          if (j >= start_pos + warmup_cooldown_points && j < end_pos - warmup_cooldown_points){
            total_points_returned += rec_vect.size();
          }
        }
        sent_count = 0;
    }

    // TimeStamp start_here = GetTimestamp();
    std::vector<int32_t> start_range = (* start_range_collection)[current_pos];
    std::vector<int32_t> end_range = (* end_range_collection)[current_pos];

    client.range_search_trie_send(start_range, end_range);

    // cout << "send a request time: " << GetTimestamp() - start_here << " for " << current_pos << endl;
    sent_count ++;

  }

  for (uint32_t j = end_pos - sent_count + 1; j <= end_pos; j++){
      std::vector<int32_t> rec_vect;
      client.range_search_trie_rec(rec_vect);

      if (j == end_pos - warmup_cooldown_points){
        diff = GetTimestamp() - start;
      }

      if (j >= start_pos + warmup_cooldown_points && j < end_pos - warmup_cooldown_points)
        total_points_returned += rec_vect.size();      
  }
  return std::make_tuple(((float) total_points_returned / diff) * 1000000, diff, total_points_to_lookup - 2 * warmup_cooldown_points);
}


std::tuple<uint32_t, float> total_client_range_search(vector<vector <int32_t>> *start_range_collection, vector<vector <int32_t>> *end_range_collection, int client_number){

  std::vector<std::future<std::tuple<uint32_t, uint32_t, uint32_t>>> threads; 
  threads.reserve(client_number);

  for (int i = 0; i < client_number; i++){

    threads.push_back(std::async(range_search_each_client, start_range_collection, end_range_collection, client_number, i));
  }  

  uint32_t total_throughput = 0;
  uint32_t total_latency = 0;
  uint32_t total_requests = 0;
  for (int i = 0; i < client_number; i++){

    std::tuple<uint32_t, uint32_t, uint32_t> return_tuple = threads[i].get();
    total_throughput += std::get<0>(return_tuple);
    total_latency += std::get<1>(return_tuple);    
    total_requests += std::get<2>(return_tuple);

    if (i == 0)
      cout << "total throughput: [" << total_throughput << "] total_latency: [" << total_latency << "] total_requests: [" << total_requests << "]" << endl;
  } 

  return std::make_tuple(total_throughput, (float) total_latency / total_requests);  
}



int main(int argc, char *argv[]){
  
  if (argc != 2 && argc != 3){
    cout << "wrong number of arguments" << endl;
    return 0;
  }

  int client_number;
  if (argc == 3){
    client_number = atoi(argv[2]);
  }

  client_number = atoi(argv[1]);

  cout << "client number: " << client_number << endl;
  auto client = MDTrieClient();

  vector<vector <int32_t>> *data_vector = get_data_vector();
 
/** 
    Insert all points from the OSM dataset
*/

  std::tuple<uint32_t, float> return_tuple = total_client_insert(data_vector, client_number);
  uint32_t throughput = std::get<0>(return_tuple);
  float latency = std::get<1>(return_tuple);

  cout << "Insertion Throughput add thread (pt / seconds): " << throughput << endl;
  cout << "Latency (us): " << latency << endl;
  cout << "Inserted Points: " << client.get_count() << endl;
  


/**  Range Search Obtain Search Range

*/
  int32_t max_range = 1 << 31;
  auto start_range = vector <int32_t>(DIMENSION, 0);
  auto end_range = vector <int32_t>(DIMENSION, max_range);

  int32_t max[DIMENSION];
  int32_t min[DIMENSION];

  for (n_leaves_t itr = 0; itr < n_lines; itr++) {

      for (dimension_t i = 0; i < DIMENSION; i++) {

          if (itr == 1) {
              max[i] = (*data_vector)[itr][i];
              min[i] = (*data_vector)[itr][i];
          } else {
              if ((*data_vector)[itr][i] > max[i]) {
                  max[i] = (*data_vector)[itr][i];
              }
              if ((*data_vector)[itr][i] < min[i]) {
                  min[i] = (*data_vector)[itr][i];
              }
          }
      }
  }  

  // int itr = 10000;
  int i = 0;

  ofstream file("range_search_size_latency.csv", std::ios_base::app);
  vector<vector <int32_t>> start_range_collection;
  vector<vector <int32_t>> end_range_collection;

  while (start_range_collection.size() < 1000){
    
    if (i % 100 == 0)
      cout << "finished: " << i << endl;
      
    for (int j = 0; j < DIMENSION; j++){

      // start_range[j] = min[j] + (max[j] - min[j] + 1) / 10 * (rand() % 10);
      // end_range[j] = start_range[j] + (max[j] - start_range[j] + 1) / 10 * (rand() % 10);
      start_range[j] = min[j] + (max[j] - min[j] + 1) / 15 * (rand() % 15);
      end_range[j] = start_range[j] + (max[j] - start_range[j] + 1) / 15 * (rand() % 15);
    }

    std::vector<int32_t> return_vect;
    TimeStamp start = GetTimestamp();
    client.range_search_trie(return_vect, start_range, end_range);
    TimeStamp diff = GetTimestamp() - start;

    // if (return_vect.size() <= 1000 && return_vect.size() > 0){
    if (return_vect.size() > 0){
      cout << "found: " << i << endl;

      start_range_collection.push_back(start_range);
      end_range_collection.push_back(end_range);

      // for (int j = 0; j < DIMENSION; j++){
      //   file << start_range[j] << ",";
      // }
      // for (int j = 0; j < DIMENSION; j++){
      //   file << end_range[j] << ",";
      // }

      file << return_vect.size() << "," << diff;
      file << endl;      
      // return 0;
    }
    i++;
  }

  file.close();

  return 0;



/** 
     Range Search 


  FILE *fp = fopen("./range_search.csv", "r");

  char *line = nullptr;
  size_t len = 0;
  ssize_t read;

  if (fp == nullptr)
  {
      fprintf(stderr, "file not found\n");
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof(cwd)) != nullptr) {
          printf("Current working dir: %s\n", cwd);
      } else {
          perror("getcwd() error");
      }
      exit(EXIT_FAILURE);
  }  

  vector<vector <int32_t>> start_range_collection;
  vector<vector <int32_t>> end_range_collection;

  while ((read = getline(&line, &len, fp)) != -1)
  {
      char *token = strtok(line, ",");

      char *ptr;
      vector <int32_t> start_range;
      vector <int32_t> end_range;
      start_range.push_back(strtoul(token, &ptr, 10));

      for (dimension_t i = 1; i < DIMENSION; i++){

        token = strtok(nullptr, ",");
        start_range.push_back(strtoul(token, &ptr, 10));
      }  

      for (dimension_t i = 0; i < DIMENSION; i++){

        token = strtok(nullptr, ",");
        end_range.push_back(strtoul(token, &ptr, 10));
      }  
      start_range_collection.push_back(start_range);
      end_range_collection.push_back(end_range);

      if (start_range_collection.size() == 200)
        break;
  }

  return_tuple = total_client_range_search(&start_range_collection, &end_range_collection, client_number);
  return_tuple = total_client_range_search(&start_range_collection, &end_range_collection, 1);

  throughput = std::get<0>(return_tuple);
  latency = std::get<1>(return_tuple);

  cout << "Range Search Throughput add thread (pt / seconds): " << throughput << endl;
  cout << "Latency (us): " << latency << endl;

  return 0;

*/

/** 
    Point Lookup from the OSM dataset
*/

  return_tuple = total_client_lookup(data_vector, client_number);
  throughput = std::get<0>(return_tuple);
  latency = std::get<1>(return_tuple);

  cout << "Primary Key Lookup Throughput add thread (pt / seconds): " << throughput << endl;
  cout << "Latency (us): " << latency << endl;

  return 0;

}