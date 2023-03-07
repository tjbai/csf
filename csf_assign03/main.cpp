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
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

struct Slot {
  unsigned int tag, access_ts, load_ts;
  bool valid;
};

bool pow2(int val, int limit = 1) {
  if (val < limit)
    return false;
  return !(val & (val - 1));
}

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

  std::vector<std::unordered_map<int, Slot>> cache(set_count);

  unsigned int time = 0;
  std::string trace;
  while (std::getline(std::cin, trace)) {
    char instruction = trace[0];
    int address = std::stoul(trace.substr(2, 10), NULL, 16);

    int index = (address >> offset_bits) & (set_count - 1);
    int tag = address >> (offset_bits + index_bits);

    // the block we care about is now cache[index][tag]

    ++time;
  }
}