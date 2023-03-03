#include <cstdio>
#include <iostream>
#include <string.h>
#include <string>

bool pow_of_two(int val, int limit) {
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

  if (!(pow_of_two(set_count, 1) && pow_of_two(blocks_per_set, 1) &&
        pow_of_two(bytes_per_block, 4))) {
    std::cerr << "Invalid set/block/byte config" << std::endl;
    return 1;
  }

  bool write_allocate = !(strcmp(argv[4], "write-allocate"));
  bool write_through = !(strcmp(argv[5], "write-through"));
  bool lru = !(strcmp(argv[6], "lru"));

  if (!write_allocate && !write_through) {
    std::cerr << "Can't combine no-write-allocate with write-back" << std::endl;
    return 1;
  }

  std::string trace;
  char instruction;
  unsigned int address;
  while (std::getline(std::cin, trace)) {
    instruction = trace[0];
    address = std::stoul(trace.substr(2, 10), nullptr, 16);
  }
}