#include "cache.h"
#include <limits>

// Write block of data from memory to cache
void Cache::write_mem_to_cache(set_t &set, unsigned int tag, int time) {
  // Evict block if set is full
  if ((int) set.size() == Cache::blocks_per_set) {
    int rep_tag = -1;
    int rep_time = std::numeric_limits<int>::max();

    // Find least recently used or created block 
    for (std::pair<int, Slot> kv : set) {
      // If LRU is true, select block with lowest last_used_ts timestamp
      if (lru && kv.second.last_used_ts < rep_time) {
        rep_tag = kv.first;
        rep_time = kv.second.last_used_ts;
      }

      // If LRU is false, select block with lowest creation_ts timestamp
      if (!lru && kv.second.creation_ts < rep_time) {
        rep_tag = kv.first;
        rep_time = kv.second.creation_ts;
      }
    }

    // If evicted block is dirty, add number of cycles to write back to memory to cycle_count
    cycle_count += set[rep_tag].dirty ? (25 * bytes_per_block) : 0;
    set.erase(rep_tag); // Remove evicted block from set
  }

  set[tag] = {time, time, false}; // Add new block to set
}

// Loads a block from cache with given index and tag and updates hit/miss and cycle count
void Cache::load_from_cache(unsigned int index, unsigned int tag, int time) {
  ++load_count;

  // Check if block is already in cache
  if (sets[index].count(tag)) {
    // Update last_used_ts timestamp and hit count
    sets[index][tag].last_used_ts = time;
    ++load_hit;
    ++cycle_count;
  }
  else {
    // Block is not in cache, load from memory
    write_mem_to_cache(sets[index], tag, time);

    // Update cycle count for miss
    cycle_count += (25 * bytes_per_block) + 1;
  }
}

// Writes a block to cache with given index and tag and updates hit/miss and cycle count
void Cache::write_to_cache(unsigned int index, unsigned int tag, int time) {
  ++store_count;

  // Check if block is already in cache
  if (sets[index].count(tag)) {
    // Update last_used_ts timestamp, dirty bit, and hit count
    sets[index][tag].last_used_ts = time;
    sets[index][tag].dirty = write_back;
    ++store_hit;

    // Update cycle count for hit
    cycle_count += 1 + (write_back ? 0 : (25 * bytes_per_block));
  }
  else {
    // Block is not in cache, write allocate or write around
    if (write_allocate) write_mem_to_cache(sets[index], tag, time);

    // Update cycle count for miss
    cycle_count += 25 * bytes_per_block + write_allocate;
  }
}

void Cache::log_cache() {
  std::cout << "Total loads: " << load_count << std::endl; 
  std::cout << "Total stores: " << store_count << std::endl;
  std::cout << "Load hits: " << load_hit << std::endl;
  std::cout << "Load misses: " << (load_count - load_hit) << std::endl;
  std::cout << "Store hits: " << store_hit << std::endl;
  std::cout << "Store misses: " << (store_count - store_hit) << std::endl;
  std::cout << "Total cycles: " << cycle_count << std::endl;
}