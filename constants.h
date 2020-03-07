
/**
 * Name: constants.h
 * Desc: Contain all types and constants for benchmark tests
 */

#pragma once

// long and int size on thinkpad verified with `getconf`
typedef unsigned long   uint64_t;
typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned char   uint8_t;

// the fun_ptr does not take an input
typedef void (*fun_ptr)();
typedef uint64_t (*mod_ptr)(uint64_t);

typedef uint64_t (*ben_ptr)(fun_ptr);
typedef uint64_t (*m_ben_ptr)(void**, int, int);

// tested on 1000 trials of 100000 iterations
const uint32_t MEASUREMENT_OVERHEAD = 40;


#if !defined(ICACHE_HITS)
    // empirically tested number of consecutive function calls for a function
    // pointer to be in I_CACHE
    const uint32_t ICACHE_HITS = 10;
#endif
