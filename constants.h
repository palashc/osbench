
/**
 * Name: constants.h
 * Desc: Contain all types and constants for benchmark tests
 */

#pragma once

// long and int size on thinkpad verified with `getconf`
typedef unsigned long   uint64_t;
typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;

// the fun_ptr does not take an input
typedef void (*fun_ptr)();

// tested on 1000 trials of 100000 iterations
const uint32_t MEASUREMENT_OVERHEAD = 38;

