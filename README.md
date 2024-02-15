# EuroSys '24 Artifact Evaluation for "Trinity: A Fast Compressed Multi-attribute Data Store"

## Folder Structure

- libmdtrie: The main code for MdTrie data structure.
  - trie.h: defines the main function calls for the data structure, such as insert_trie and range_search_trie.  
  - tree_block.h: defines the function calls for each treeblock, including insert, child and range_search_treeblock, select_subtree selects a node and its subtree to turn into a frontier node.    
  - trie_node.h: MdTrie in this implementation includes a top-level pointer-based trie data structure, where each trie node, defined in trie_node.h, stores an array of pointers to children trie nodes. Its leaves point to child treeblocks.  
  - compressed_bitmap.h: defines the structure of the bit vector in the treeblocks and supports collapsed nodes.  
  - compact_ptr.h: implements how primary keys are stored at the leaf, either stored directly as value, in a vector, or a delta-encoded array (defined in delta_encoded_array.h)  
- librpc: Trinity implementation based on Thrift
- modules: Cmake dependencies for the code
- scripts: Scripts used to run the evaluations. 
- thrift: Thrift source files. 

## Hardware and OS

No special hardware requirement. We used node `xl170` on Ubuntu 20.04. 

An example Cloudlab Profile is attached in `scripts/cloudlab_profile.py`. 

## Setup

```bash
bash scripts/setup_one_node.sh
```

To run a distributed setup, execute the setup scripts on all client and storage nodes. 

## Build

```bash
mkdir -p build
cd build
cmake -DGENERATE_THRIFT=on -Dbuild_rpc=on ..
make
```

## Contact

Ziming Mao (ziming.mao@berkeley.edu)

## License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
