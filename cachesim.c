/**
 * @author ECE 3057 TAs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cachesim.h"

// Statistics you will need to keep track. DO NOT CHANGE THESE.
counter_t accesses = 0;     // Total number of cache accesses
counter_t hits = 0;         // Total number of cache hits
counter_t misses = 0;       // Total number of cache misses
counter_t writebacks = 0;   // Total number of writebacks

/**
 * Function to perform a very basic log2. It is not a full log function,
 * but it is all that is needed for this assignment. The <math.h> log
 * function causes issues for some people, so we are providing this.
 *
 * @param x is the number you want the log of.
 * @returns Techinically, floor(log_2(x)). But for this lab, x should always be a power of 2.
 */
int simple_log_2(int x) {
    int val = 0;
    while (x > 1) {
        x /= 2;
        val++;
    }
    return val;
}

//  Here are some global variables you may find useful to get you started.
//      Feel free to add/change anyting here.
cache_set_t* cache;     // Data structure for the cache
int block_size;         // Block size
int cache_size;         // Cache size
int ways;               // Ways
int num_sets;           // Number of sets
int num_offset_bits;    // Number of offset bits
int num_index_bits;     // Number of index bits.

/**
 * Function to intialize your cache simulator with the given cache parameters.
 * Note that we will only input valid parameters and all the inputs will always
 * be a power of 2.
 *
 * @param _block_size is the block size in bytes
 * @param _cache_size is the cache size in bytes
 * @param _ways is the associativity
 */
void cachesim_init(int _block_size, int _cache_size, int _ways) {
    // Set cache parameters to global variables
    block_size = _block_size;
    cache_size = _cache_size;
    ways = _ways;

    ////////////////////////////////////////////////////////////////////
    //  TODO: Write the rest of the code needed to initialize your cache
    //  simulator. Some of the things you may want to do are:
    //      - Calculate any values you need such as number of index bits.
    //      - Allocate any data structures you need.
    ////////////////////////////////////////////////////////////////////
    num_sets = cache_size / block_size;
    num_offset_bits = simple_log_2(num_sets);
    num_index_bits = simple_log_2(block_size);
    cache = (cache_set_t*)malloc(sizeof(cache_set_t) * cache_size);
    cache->blocks = (cache_block_t*)malloc(block_size * num_sets * ways * sizeof(cache_block_t));
    cache->stack = init_lru_stack(ways);
    ////////////////////////////////////////////////////////////////////
    //  End of your code
    ////////////////////////////////////////////////////////////////////
}

/**
 * Function to perform a SINGLE memory access to your cache. In this function,
 * you will need to update the required statistics (accesses, hits, misses, writebacks)
 * and update your cache data structure with any changes necessary.
 *
 * @param physical_addr is the address to use for the memory access.
 * @param access_type is the type of access - 0 (data read), 1 (data write) or
 *      2 (instruction read). We have provided macros (MEMREAD, MEMWRITE, IFETCH)
 *      to reflect these values in cachesim.h so you can make your code more readable.
 */
void cachesim_access(addr_t physical_addr, int access_type) {
    ////////////////////////////////////////////////////////////////////
    //  TODO: Write the code needed to perform a cache access on your
    //  cache simulator. Some things to remember:
    //      - When it is a cache hit, you SHOULD NOT bring another cache
    //        block in.
    //      - When it is a cache miss, you should bring a new cache block
    //        in. If the set is full, evict the LRU block.
    //      - Remember to update all the necessary statistics as necessary
    //      - Remember to correctly update your valid and dirty bits.
    ////////////////////////////////////////////////////////////////////
    counter_t tag = (physical_addr & 128);
    counter_t index = (physical_addr & 112);

    /*
    *   Update LRU, MRU and tag information in the cache
    */
    int mru = getMRU(cache->stack);
    int lru = getMRU(cache->stack);
    int hitIndex = find(cache->stack, tag);

    for(int i = 0; i < ways; i++) {
      if(cache->blocks[index][i].valid == 1) {
        // hit
        if(cache->blocks[index][i].tag == tag) {
          // filled hit
          if(isFull(cache->stack)) {
            switch(access_type) {
              case MEMWRITE :
                cache->stack.lru_stack_set_mru(cache->stack, i);
                hits++;
                break;
              case IFETCH :
                cache->stack.lru_stack_set_mru(cache->stack, i);
                hits++;
                break;
              case MEMREAD :
                cache->stack.lru_stack_set_mru(cache->stack, i);
                hits++;
                break;
            }
          }
          // unfilled hit
          else {
            switch(access_type) {
              case MEMWRITE :
              cache->stack.lru_stack_set_mru(cache->stack, i);
              hits++;
              break;
            case IFETCH :
              cache->stack.lru_stack_set_mru(cache->stack, i);
              hits++;
              break;
            case MEMREAD :
              cache->stack.lru_stack_set_mru(cache->stack, i);
              hits++;
              break;
            }
          }
          accessess++;
          return;
        }
      }
      // not filled cache memory
      else {
        if(!cache->stack.isFull()) {
          switch(access_type) {
            case MEMWRITE :
            cache->blocks[index][i].tag = tag;
            cache->blocks[index][i].valid = 1;
            cache->blocks[index][i].dirty = 0;
            cache->stack.lru_stack_set_mru(cache->stack, i);
            writebacks++;
            break;
            case IFETCH:
            cache->blocks[index][i].tag = tag;
            cache->blocks[index][i].valid = 1;
            cache->blocks[index][i].dirty = 0;
            cache->stack.lru_stack_set_mru(cache->stack, i);
            misses++;
            break;
            case MEMREAD :
            cache->blocks[index][i].tag = tag;
            cache->blocks[index][i].valid = 1;
            cache->blocks[index][i].dirty = 0;
            cache->stack.lru_stack_set_mru(cache->stack, i);
            misses++;
            break;
          }
        }
        accessess++;
        return;
      }
    }

    // filled miss
    switch(access_type) {
      case MEMWRITE :
      cache->blocks[index][getLRU(cache->stack)].tag = tag;
      cache->blocks[index][getLRU(cache->stack)].valid = 1;
      cache->blocks[index][getLRU(cache->stack)].dirty = 1;
      cache->stack.lru_stack_set_mru(cache->stack, getLRU(cache->stack));
      writebacks++;
      break;
      case IFETCH:
      cache->blocks[index][getLRU(cache->stack)].tag = tag;
      cache->blocks[index][getLRU(cache->stack)].valid = 1;
      cache->blocks[index][getLRU(cache->stack)].dirty = 1;
      cache->stack.lru_stack_set_mru(cache->stack, getLRU(cache->stack));
      misses++;
      break;
      case MEMREAD :
      cache->blocks[index][getLRU(cache->stack)].tag = tag;
      cache->blocks[index][getLRU(cache->stack)].valid = 1;
      cache->blocks[index][getLRU(cache->stack)].dirty = 1;
      cache->stack.lru_stack_set_mru(cache->stack, getLRU(cache->stack));
      misses++;
      break;
    }
    accesses++;
    ////////////////////////////////////////////////////////////////////
    //  End of your code
    ////////////////////////////////////////////////////////////////////
}

/**
 * Function to free up any dynamically allocated memory you allocated
 */
void cachesim_cleanup() {
    ////////////////////////////////////////////////////////////////////
    //  TODO: Write the code to do any heap allocation cleanup
    ////////////////////////////////////////////////////////////////////
    free(cache->stack);
    free(cache->blocks);
    free(cache);
    ////////////////////////////////////////////////////////////////////
    //  End of your code
    ////////////////////////////////////////////////////////////////////
}

/**
 * Function to print cache statistics
 * DO NOT update what this prints.
 */
void cachesim_print_stats() {
    printf("%llu, %llu, %llu, %llu\n", accesses, hits, misses, writebacks);
}

/**
 * Function to open the trace file
 * You do not need to update this function.
 */
FILE *open_trace(const char *filename) {
    return fopen(filename, "r");
}

/**
 * Read in next line of the trace
 *
 * @param trace is the file handler for the trace
 * @return 0 when error or EOF and 1 otherwise.
 */
int next_line(FILE* trace) {
    if (feof(trace) || ferror(trace)) return 0;
    else {
        int t;
        unsigned long long address, instr;
        fscanf(trace, "%d %llx %llx\n", &t, &address, &instr);
        cachesim_access(address, t);
    }
    return 1;
}

/**
 * Main function. See error message for usage.
 *
 * @param argc number of arguments
 * @param argv Argument values
 * @returns 0 on success.
 */
int main(int argc, char **argv) {
    FILE *input;

    if (argc != 5) {
        fprintf(stderr, "Usage:\n  %s <trace> <block size(bytes)>"
                        " <cache size(bytes)> <ways>\n", argv[0]);
        return 1;
    }

    input = open_trace(argv[1]);
    cachesim_init(atol(argv[2]), atol(argv[3]), atol(argv[4]));
    while (next_line(input));
    cachesim_print_stats();
    cachesim_cleanup();
    fclose(input);
    return 0;
}
