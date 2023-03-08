/*
 * Cache simulation main function
 * CSF Assignment 3
 * TJ Bai
 * tbai4@jhu.edu
 * Halle Cho
 * hcho64@jhu.edu
 */

#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

struct Slot {
  int access_ts, load_ts;
  bool dirty = false;
};

typedef std::unordered_map<int, Slot> set_t;
typedef std::vector<set_t> cache_t;

bool pow2(int val, int limit = 1);
int erase_from_full_set(set_t &set, bool lru);
void load_into_cache(set_t &set, int max_size, int time, 
                    int tag, bool lru, int &cycle_count);

int main(int argc, char *argv[]) {
  if (argc != 7) {
    std::cerr << "Invalid argument count" << std::endl;
    return 1;
  }

  int set_count = std::stoi(argv[1]);
  int blocks_per_set = std::stoi(argv[2]);
  int bytes_per_block = std::stoi(argv[3]);

  if (!(pow2(set_count) && pow2(blocks_per_set) && pow2(bytes_per_block, 4))) {
    std::cerr << "Invalid set/block/byte config" << std::endl;
    return 1;
  }

  bool write_allocate = !(strcmp(argv[4], "write-allocate"));
  bool write_back = !(strcmp(argv[5], "write-back"));
  bool lru = !(strcmp(argv[6], "lru"));

  if (!write_allocate && write_back) {
    std::cerr << "Can't combine no-write-allocate with write-back" << std::endl;
    return 1;
  }

  int offset_bits = log2(bytes_per_block);
  int index_bits = log2(set_count);

  cache_t cache(set_count);
  int load_count = 0, load_hit = 0;
  int store_count = 0, store_hit = 0;
  int cycle_count = 0;

  unsigned int time = 0;
  std::string trace;

  // TODO: increment clock cycles
  while (std::getline(std::cin, trace)) {
    char instruction = trace[0];
    int address = std::stoul(trace.substr(2, 10), NULL, 16);

    int index = (address >> offset_bits) & (set_count - 1);
    int tag = address >> (offset_bits + index_bits);

    if (instruction == 'l') {
      ++load_count;
      if (cache[index].count(tag)) {
        ++load_hit;
      }
      else {
        load_into_cache(cache[index], blocks_per_set, time, tag, lru, cycle_count);
      }
    } else {
      if (cache[index].count(tag)) continue;
      else continue;
    }

    ++time;
  }
}

bool pow2(int val, int limit) {
  if (val < limit) return false;
  return !(val & (val - 1));
}

int erase_from_full_set(set_t &set, bool lru, int &cycle_count) {
  // TODO: check for "dirty" bit

  int rep_tag = -1;
  int last_used_time = std::numeric_limits<int>::max();

  for (std::pair<int, Slot> kv : set) {
    if (lru && kv.second.access_ts < last_used_time) {
      rep_tag = kv.first;
      last_used_time = kv.second.access_ts;
    }

    if (!lru && kv.second.load_ts < last_used_time) {
      rep_tag = kv.first;
      last_used_time = kv.second.load_ts;
    }
  }

  set.erase(rep_tag);
  return rep_tag;
} 

void load_into_cache(set_t &set, int max_size, int time, 
                    int tag, bool lru, int &cycle_count) {
  if (set.size() == max_size) erase_from_full_set(set, lru);
  set[tag] = {time, time};
}
