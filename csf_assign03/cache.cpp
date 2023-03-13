#include "cache.h"
#include <limits>

/*
 * Write block of data from memory to cache
 *
 * Parameters:
 *   set - memory address of set map
 *   tag - tag of data block
 *   time - timestamp of call
 *
 */
void Cache::write_mem_to_cache(set_t &set, unsigned int tag, int time) {
  if ((int) set.size() == Cache::blocks_per_set) {
    int rep_tag = -1;
    int rep_time = std::numeric_limits<int>::max();

    for (std::pair<int, Slot> kv : set) {
      if (lru && kv.second.last_used_ts < rep_time) {
        rep_tag = kv.first;
        rep_time = kv.second.last_used_ts;
      }

      if (!lru && kv.second.creation_ts < rep_time) {
        rep_tag = kv.first;
        rep_time = kv.second.creation_ts;
      }
    }

    cycle_count += set[rep_tag].dirty ? (25 * bytes_per_block) : 0;
    set.erase(rep_tag);
  }

  set[tag] = {time, time, false}; 
}

/*
 * Loads a black from cache with given index and tag and updates hit/miss and cycle count
 *
 * Parameters:
 *   index - index in set
 *   tag - tag of data block
 *   time - timestamp
 *
 */
void Cache::load_from_cache(unsigned int index, unsigned int tag, int time) {
  ++load_count;

  if (sets[index].count(tag)) {
    sets[index][tag].last_used_ts = time;
    ++load_hit;
    ++cycle_count;
  }
  else {
    write_mem_to_cache(sets[index], tag, time);
    cycle_count += (25 * bytes_per_block) + 1;
  }
}

/*
 * Writes a block of data to cache with given index and tag and updates hit/miss and cycle count
 *
 * Parameters:
 *   index - index of block in set
 *   tag - tag of data block
 *   time - timestamp
 *
 */
void Cache::write_to_cache(unsigned int index, unsigned int tag, int time) {
  ++store_count;

  if (sets[index].count(tag)) {
    sets[index][tag].last_used_ts = time;
    sets[index][tag].dirty = write_back;
    ++store_hit;
    cycle_count += 1 + (write_back ? 0 : (25 * bytes_per_block));
  }
  else {
    if (write_allocate) write_mem_to_cache(sets[index], tag, time);
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