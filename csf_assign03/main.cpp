/*
 * Cache simulation main function
 * CSF Assignment 3
 * TJ Bai
 * tbai4@jhu.edu
 * Halle Cho
 * hcho64@jhu.edu
 */

#include <cmath>
#include <iostream>
#include <string.h>
#include <string>
#include "cache.h"

bool check_pow2(int val, int limit = 1);

int main(int argc, char *argv[]) {
  if (argc != 7) {
    std::cerr << "Invalid argument count" << std::endl;
    return 1;
  }

  int set_count = std::stoi(argv[1]);
  int blocks_per_set = std::stoi(argv[2]);
  int bytes_per_block = std::stoi(argv[3]);

  if (!(
    check_pow2(set_count) && 
    check_pow2(blocks_per_set) && 
    check_pow2(bytes_per_block, 4)
  )) {
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
  int time = 0;

  Cache cache(
    set_count,
    blocks_per_set,
    bytes_per_block,
    write_allocate,
    write_back,
    lru
  );

  std::string trace;
  unsigned int address, index, tag;
  while (std::getline(std::cin, trace)) {
    char instruction = trace[0];
    address = std::stoul(trace.substr(2, 10), NULL, 16);
    index = (address >> offset_bits) & (set_count - 1);
    tag = address >> (offset_bits + index_bits);

    if (instruction == 'l') cache.load_from_cache(index, tag, time);
    else cache.write_to_cache(index, tag, time);

    ++time;
  }
  
  cache.log_cache();
}

bool check_pow2(int val, int limit) {
  if (val < limit) return false;
  return !(val & (val - 1));
}
