#ifndef CACHE_H 
#define CACHE_H 

#include <vector>
#include <unordered_map>
#include <iostream>

// Define struct to represent cache slot
struct Slot { int last_used_ts, creation_ts; bool dirty; };

// Unordered map with int and Slot values
typedef std::unordered_map<int, Slot> set_t;

// Define class
class Cache {
  private:
    // Private data members to configure cache behavior
    int set_count, blocks_per_set, bytes_per_block;
    bool write_allocate, write_back, lru;

    // Performance statistics
    int load_count = 0, load_hit = 0;
    int store_count = 0, store_hit = 0;
    unsigned int cycle_count = 0;

    std::vector<set_t> sets;

    // Writes data from memory to cache
    void write_mem_to_cache(set_t &set, unsigned int tag, int time);

  public:
    Cache(
      int set_count,
      int blocks_per_set,
      int bytes_per_block, 
      bool write_allocate, 
      bool write_back, 
      bool lru
    ) {
      this->sets = std::vector<set_t>(set_count);
      this->set_count = set_count;
      this->blocks_per_set = blocks_per_set;
      this->bytes_per_block = bytes_per_block;
      this->write_allocate = write_allocate;
      this->write_back = write_back;
      this->lru = lru;
    };

    // Loading and writing data to cache
    void load_from_cache(unsigned int index, unsigned int tag, int time);
    void write_to_cache(unsigned int index, unsigned int tag, int time);

    // Log performance statistics
    void log_cache();
};

#endif