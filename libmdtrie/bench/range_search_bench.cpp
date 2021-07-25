#include "trie.h"
#include <unistd.h>
#include <sys/time.h>
#include <climits>
#include <tqdm.h>

const int DIMENSION = 9;
FILE *fptr;
char file_path[] = "benchmark_range_search_9.csv";

typedef unsigned long long int TimeStamp;
static TimeStamp GetTimestamp() {
  struct timeval now;
  gettimeofday(&now, nullptr);

  return now.tv_usec + (TimeStamp) now.tv_sec * 1000000;
}


void insert_for_node_path(point_array<DIMENSION> *found_points, level_t max_depth, level_t trie_depth, preorder_t max_tree_node){
    // to-do
    
    auto *mdtrie = new md_trie<DIMENSION>(max_depth, trie_depth, max_tree_node);
    auto *leaf_point = new data_point<DIMENSION>();

    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fopen("../libmdtrie/bench/data/sample_shuf.txt", "r");

    // If the file cannot be open
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
    // TimeStamp start, diff;
    
    n_leaves_t n_points = 0;
    uint64_t max[DIMENSION];
    uint64_t min[DIMENSION];
    n_leaves_t n_lines = 14583357;
    // diff = 0;
    tqdm bar;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        bar.progress(n_points, n_lines);
        char *token = strtok(line, " ");
        char *ptr;
        for (uint8_t i = 0; i < 2; i ++){
            token = strtok(nullptr, " ");
        }
        for (dimension_t i = 0; i < DIMENSION; i++){
            if (i >= 4){
                leaf_point->set_coordinate(i, leaf_point->get_coordinate(i % 4));
            }
            else {
                token = strtok(nullptr, " ");
                leaf_point->set_coordinate(i, strtoul(token, &ptr, 10));
            }
            if (n_points == 0){
                max[i] = leaf_point->get_coordinate(i);
                min[i] = leaf_point->get_coordinate(i);
            }
            else {
                if (leaf_point->get_coordinate(i) > max[i]){
                    max[i] = leaf_point->get_coordinate(i);
                }
                if (leaf_point->get_coordinate(i) < min[i]){
                    min[i] = leaf_point->get_coordinate(i);
                }
            }
        }
        mdtrie->insert_trie(leaf_point, max_depth);
        n_points ++;
    }
    bar.finish();
    // uint64_t msec;
    fprintf(stderr, "md-trie size: %ld\n", mdtrie->size());   
    
    auto *start_range = new data_point<DIMENSION>();
    auto *end_range = new data_point<DIMENSION>();
    
    // int itr = 1;

    // uint64_t total_found_points = 0;
    // TimeStamp lookup_time = 0;
    while (found_points->size() == 0){
        for (dimension_t i = 0; i < DIMENSION; i++){
            start_range->set_coordinate(i,  min[i] + rand() % (max[i] - min[i] + 1));
            end_range->set_coordinate(i, start_range->get_coordinate(i) + rand() % (max[i] - start_range->get_coordinate(i) + 1));
        }
        // start = GetTimestamp();
        mdtrie->range_search_trie(start_range, end_range, mdtrie->root(), 0, found_points);
    }
    // diff = GetTimestamp() - start;
    // if (found_points->size() == 0){
    //     continue;
    // }

    // msec = diff * 1000 / CLOCKS_PER_SEC;
    // total_found_points += found_points->size();
    // lookup_time = test_node_path_only(found_points, max_depth);

        
    // msec = lookup_time * 1000 / CLOCKS_PER_SEC;
    // fprintf(stderr, "Time per Checking: %f, out of %ld points\n", (float)msec*1000 / total_found_points, total_found_points);
}

void test_node_path_only(level_t max_depth, level_t trie_depth, preorder_t max_tree_node){
    auto *found_points = new point_array<DIMENSION>();
    insert_for_node_path(found_points, max_depth, trie_depth, max_tree_node);
    // while (found_points->size() == 0){
    //     found_points = insert_for_node_path(max_depth, trie_depth, max_tree_node);
    // }

    TimeStamp diff = 0;
    TimeStamp start;
    n_leaves_t found_points_size = found_points->size();
    
    for (n_leaves_t i = 0; i < found_points_size; i++){
        
        symbol_t *node_path = (symbol_t *)malloc((max_depth + 1) * sizeof(symbol_t));
        data_point<DIMENSION> *point = found_points->at(i);
        tree_block<DIMENSION> *parent_treeblock = point->get_parent_treeblock();
        symbol_t parent_symbol = point->get_parent_symbol();
        node_t parent_node = point->get_parent_node();

        start = GetTimestamp();
        parent_treeblock->get_node_path(parent_node, node_path); 

        node_path[max_depth - 1] = parent_symbol;
        
        auto returned_coordinates = parent_treeblock->node_path_to_coordinates(node_path);
        diff += GetTimestamp() - start;

        for (dimension_t i = 0; i < DIMENSION; i++){
            if (returned_coordinates->get_coordinate(i) != point->get_coordinate(i)){
                raise(SIGINT);
            }
        }
        free(node_path);
    }
    // return diff;
    uint64_t msec;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    fprintf(stderr, "Time per Checking: %f, out of %ld points\n", (float)msec*1000 / found_points->size(), found_points->size());
     
}

TimeStamp get_node_path_time(point_array<DIMENSION> *found_points, level_t max_depth){
    // auto *found_points = new point_array<DIMENSION>();
    // insert_for_node_path(found_points, max_depth, trie_depth, max_tree_node);
    // while (found_points->size() == 0){
    //     found_points = insert_for_node_path(max_depth, trie_depth, max_tree_node);
    // }

    TimeStamp diff = 0;
    TimeStamp start;
    n_leaves_t found_points_size = found_points->size();
    
    for (n_leaves_t i = 0; i < found_points_size; i++){
        
        symbol_t *node_path = (symbol_t *)malloc((max_depth + 1) * sizeof(symbol_t));
        data_point<DIMENSION> *point = found_points->at(i);
        tree_block<DIMENSION> *parent_treeblock = point->get_parent_treeblock();
        symbol_t parent_symbol = point->get_parent_symbol();
        node_t parent_node = point->get_parent_node();

        start = GetTimestamp();
        parent_treeblock->get_node_path(parent_node, node_path); 

        node_path[max_depth - 1] = parent_symbol;
        
        auto returned_coordinates = parent_treeblock->node_path_to_coordinates(node_path);
        diff += GetTimestamp() - start;

        for (dimension_t i = 0; i < DIMENSION; i++){
            if (returned_coordinates->get_coordinate(i) != point->get_coordinate(i)){
                raise(SIGINT);
            }
        }

        free(node_path);
    }
    return diff;
    // uint64_t msec;
    // msec = diff * 1000 / CLOCKS_PER_SEC;
    // fprintf(stderr, "Time per Checking: %f, out of %ld points\n", (float)msec*1000 / found_points->size(), found_points->size());    
}

void test_real_data(level_t max_depth, level_t trie_depth, preorder_t max_tree_node, int n_itr){
    // to-do
    
    fptr = fopen(file_path, "a");
    fprintf(fptr, "Number of Points, Volume, Latency, %d\n", DIMENSION);
    auto *mdtrie = new md_trie<DIMENSION>(max_depth, trie_depth, max_tree_node);
    auto *leaf_point = new data_point<DIMENSION>();

    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fopen("../libmdtrie/bench/data/sample_shuf.txt", "r");

    // If the file cannot be open
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
    TimeStamp start, diff;
    
    n_leaves_t n_points = 0;
    uint64_t max[DIMENSION];
    uint64_t min[DIMENSION];
    n_leaves_t n_lines = 14583357;
    diff = 0;
    tqdm bar;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        bar.progress(n_points, n_lines);
        char *token = strtok(line, " ");
        char *ptr;
        for (uint8_t i = 0; i < 2; i ++){
            token = strtok(nullptr, " ");
        }
        for (dimension_t i = 0; i < DIMENSION; i++){
            if (i >= 4){
                leaf_point->set_coordinate(i, leaf_point->get_coordinate(i % 4));
            }
            else {
                token = strtok(nullptr, " ");
                leaf_point->set_coordinate(i, strtoul(token, &ptr, 10));
            }
            if (n_points == 0){
                max[i] = leaf_point->get_coordinate(i);
                min[i] = leaf_point->get_coordinate(i);
            }
            else {
                if (leaf_point->get_coordinate(i) > max[i]){
                    max[i] = leaf_point->get_coordinate(i);
                }
                if (leaf_point->get_coordinate(i) < min[i]){
                    min[i] = leaf_point->get_coordinate(i);
                }
            }
        }
        mdtrie->insert_trie(leaf_point, max_depth);
        n_points ++;
    }
    bar.finish();

    
    auto *start_range = new data_point<DIMENSION>();
    auto *end_range = new data_point<DIMENSION>();
    auto *found_points = new point_array<DIMENSION>();
    int itr = 1;
    tqdm bar1;
    uint64_t msec;
    uint64_t total_found_points = 0;
    TimeStamp lookup_time = 0;
    while (itr <= n_itr){
        bar1.progress(itr, n_itr);
        uint64_t volume = 1;
        for (dimension_t i = 0; i < DIMENSION; i++){
            start_range->set_coordinate(i,  min[i] + rand() % (max[i] - min[i] + 1));
            end_range->set_coordinate(i, start_range->get_coordinate(i) + rand() % (max[i] - start_range->get_coordinate(i) + 1));
            volume *= (end_range->get_coordinate(i) - start_range->get_coordinate(i));
        }
        start = GetTimestamp();
        mdtrie->range_search_trie(start_range, end_range, mdtrie->root(), 0, found_points);
        diff = GetTimestamp() - start;
        if (found_points->size() == 0){
            continue;
        }
        // uint64_t msec;

        msec = diff * 1000 / CLOCKS_PER_SEC;
        total_found_points += found_points->size();
        lookup_time += get_node_path_time(found_points, max_depth);
        fprintf(fptr, "%ld, %ld, %f\n", found_points->size(), volume, (float)msec*1000);
        
        found_points->reset();
        itr++;
        fclose(fptr);
        fptr = fopen(file_path, "a");            
    }
    bar1.finish();
    fprintf(stderr, "dimension: %d", DIMENSION);
    fprintf(stderr, "md-trie size: %ld\n", mdtrie->size()); 
    msec = lookup_time * 1000 / CLOCKS_PER_SEC;
    fprintf(stderr, "Time per Checking: %f, out of %ld points\n", (float)msec*1000 / total_found_points, total_found_points);
    fclose(fptr);
}



void range_search_insert(md_trie<DIMENSION> *mdtrie, level_t max_depth, uint64_t * max, uint64_t *min){
    auto *leaf_point = new data_point<DIMENSION>();

    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fopen("../libmdtrie/bench/data/sample_shuf.txt", "r");

    // If the file cannot be open
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
    // TimeStamp start, diff;
    
    n_leaves_t n_points = 0;
    n_leaves_t n_lines = 14583357;
    // diff = 0;
    tqdm bar;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        bar.progress(n_points, n_lines);
        char *token = strtok(line, " ");
        char *ptr;
        for (uint8_t i = 0; i < 2; i ++){
            token = strtok(nullptr, " ");
        }
        for (dimension_t i = 0; i < DIMENSION; i++){
            if (i >= 4){
                leaf_point->set_coordinate(i, leaf_point->get_coordinate(i % 4));
            }
            else {
                token = strtok(nullptr, " ");
                leaf_point->set_coordinate(i, strtoul(token, &ptr, 10));
            }
            if (n_points == 0){
                max[i] = leaf_point->get_coordinate(i);
                min[i] = leaf_point->get_coordinate(i);
            }
            else {
                if (leaf_point->get_coordinate(i) > max[i]){
                    max[i] = leaf_point->get_coordinate(i);
                }
                if (leaf_point->get_coordinate(i) < min[i]){
                    min[i] = leaf_point->get_coordinate(i);
                }
            }
        }
        mdtrie->insert_trie(leaf_point, max_depth);
        n_points ++;
    }
    bar.finish();
}

void test_range_only(level_t max_depth, level_t trie_depth, preorder_t max_tree_node, int n_itr){

    uint64_t max[DIMENSION];
    uint64_t min[DIMENSION];

    auto *mdtrie = new md_trie<DIMENSION>(max_depth, trie_depth, max_tree_node);
    range_search_insert(mdtrie, max_depth, max, min);

    auto *start_range = new data_point<DIMENSION>();
    auto *end_range = new data_point<DIMENSION>();
    auto *found_points = new point_array<DIMENSION>();
    int itr = 1;
    // TODO: warm up the cache. First 10%
    while (itr <= n_itr){
        uint64_t volume = 1;
        for (dimension_t i = 0; i < DIMENSION; i++){
            start_range->set_coordinate(i,  min[i] + rand() % (max[i] - min[i] + 1));
            end_range->set_coordinate(i, start_range->get_coordinate(i) + rand() % (max[i] - start_range->get_coordinate(i) + 1));
            volume *= (end_range->get_coordinate(i) - start_range->get_coordinate(i));
        }
        mdtrie->range_search_trie(start_range, end_range, mdtrie->root(), 0, found_points);
        if (found_points->size() == 0){
            continue;
        }
        found_points->reset();
        itr++;          
    }
}

bool test_random_range_search(n_leaves_t n_points, level_t max_depth, level_t trie_depth,
                       preorder_t max_tree_nodes, uint32_t n_itr = 50) {
    auto range = (symbol_t) pow(2, max_depth);
    auto *mdtrie = new md_trie<DIMENSION>(max_depth, trie_depth, max_tree_nodes);
    auto *leaf_point = new data_point<DIMENSION>();
    uint64_t max[DIMENSION];
    uint64_t min[DIMENSION];

    for (n_leaves_t itr = 1; itr <= n_points; itr++) {

        for (dimension_t i = 0; i < DIMENSION; i++) {
            leaf_point->set_coordinate(i, rand() % range);
            if (itr == 1) {
                max[i] = leaf_point->get_coordinate(i);
                min[i] = leaf_point->get_coordinate(i);
            } else {
                if (leaf_point->get_coordinate(i) > max[i]) {
                    max[i] = leaf_point->get_coordinate(i);
                }
                if (leaf_point->get_coordinate(i) < min[i]) {
                    min[i] = leaf_point->get_coordinate(i);
                }
            }
        }
        mdtrie->insert_trie(leaf_point, max_depth);
    }
    auto *start_range = new data_point<DIMENSION>();
    auto *end_range = new data_point<DIMENSION>();
    auto *found_points = new point_array<DIMENSION>();
    // tqdm bar1;
    for (uint32_t itr = 1; itr <= n_itr; itr++) {
        // bar1.progress(itr, n_itr);
        for (dimension_t i = 0; i < DIMENSION; i++) {
            start_range->set_coordinate(i, min[i] + rand() % (max[i] - min[i] + 1));
            end_range->set_coordinate(i, start_range->get_coordinate(i) + rand() % (max[i] - start_range->get_coordinate(i) + 1));
        }

        mdtrie->range_search_trie(start_range, end_range, mdtrie->root(), 0, found_points);
        found_points->reset();
    }
    // bar1.finish();
    return true;
}



void test_search_one_dimension(level_t max_depth, level_t trie_depth, preorder_t max_tree_node, int n_itr){
    // to-do
    
    fptr = fopen(file_path, "a");
    fprintf(fptr, "Number of Points, Volume, Latency, %d\n", DIMENSION);
    auto *mdtrie = new md_trie<DIMENSION>(max_depth, trie_depth, max_tree_node);
    auto *leaf_point = new data_point<DIMENSION>();
    uint64_t upper_bound = pow(2, max_depth) - 1;

    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fopen("../libmdtrie/bench/data/sample_shuf.txt", "r");

    // If the file cannot be open
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
    TimeStamp start, diff;
    
    n_leaves_t n_points = 0;
    uint64_t max[DIMENSION];
    uint64_t min[DIMENSION];
    n_leaves_t n_lines = 14583357;
    diff = 0;
    tqdm bar;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        bar.progress(n_points, n_lines);
        char *token = strtok(line, " ");
        char *ptr;
        for (uint8_t i = 0; i < 2; i ++){
            token = strtok(nullptr, " ");
        }
        for (dimension_t i = 0; i < DIMENSION; i++){
            if (i >= 4){
                leaf_point->set_coordinate(i, leaf_point->get_coordinate(i % 4));
            }
            else {
                token = strtok(nullptr, " ");
                leaf_point->set_coordinate(i, strtoul(token, &ptr, 10));
            }
            if (n_points == 0){
                max[i] = leaf_point->get_coordinate(i);
                min[i] = leaf_point->get_coordinate(i);
            }
            else {
                if (leaf_point->get_coordinate(i) > max[i]){
                    max[i] = leaf_point->get_coordinate(i);
                }
                if (leaf_point->get_coordinate(i) < min[i]){
                    min[i] = leaf_point->get_coordinate(i);
                }
            }
        }
        mdtrie->insert_trie(leaf_point, max_depth);
        n_points ++;
    }
    bar.finish();

    
    auto *start_range = new data_point<DIMENSION>();
    auto *end_range = new data_point<DIMENSION>();
    auto *found_points = new point_array<DIMENSION>();
    int itr = 1;
    tqdm bar1;
    uint64_t msec;
    uint64_t total_found_points = 0;
    TimeStamp lookup_time = 0;

    while (itr <= n_itr){
        bar1.progress(itr, n_itr);
        uint64_t volume = 1;
        for (dimension_t j = 0; j < DIMENSION; j++){
            for (dimension_t i = 0; i < DIMENSION; i++){
                if (i == j){
                    start_range->set_coordinate(i,  min[i] + rand() % (max[i] - min[i] + 1));
                    end_range->set_coordinate(i, start_range->get_coordinate(i) + rand() % (max[i] - start_range->get_coordinate(i) + 1));
                    
                }
                else {
                    start_range->set_coordinate(i, 0);
                    end_range->set_coordinate(i, upper_bound);
                    volume *= (end_range->get_coordinate(i) - start_range->get_coordinate(i));
                }
            }
        

            start = GetTimestamp();
            mdtrie->range_search_trie(start_range, end_range, mdtrie->root(), 0, found_points);
            diff = GetTimestamp() - start;
            if (found_points->size() == 0){
                continue;
            }

            msec = diff * 1000 / CLOCKS_PER_SEC;
            total_found_points += found_points->size();
            lookup_time += get_node_path_time(found_points, max_depth);
            fprintf(fptr, "%ld, %ld, %f\n", found_points->size(), volume, (float)msec*1000);
            
            found_points->reset();
            itr++;         
        } 
    }
    bar1.finish();
    fprintf(stderr, "dimension: %d", DIMENSION);
    fprintf(stderr, "md-trie size: %ld\n", mdtrie->size()); 
    msec = lookup_time * 1000 / CLOCKS_PER_SEC;
    fprintf(stderr, "Time per Checking: %f, out of %ld points\n", (float)msec*1000 / total_found_points, total_found_points);
    fclose(fptr);
}


// int dimensions, level_type max_depth, level_type trie_depth, preorder_type max_tree_node, int n_itr
int main() {
    srand(static_cast<unsigned int>(time(0)));
    
    test_search_one_dimension(32, 10, 1024, 100);
    // test_range_only(32, 10, 1024, 50);
    // test_real_data(32, 10, 1024, 50);

}