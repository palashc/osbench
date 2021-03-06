
#pragma once

#include "constants.h"
#include "math.h"
#include "limits.h"
#include <stdlib.h>

// this sorts the input parameter and approximates median by returning the middle
// of the array
int compare (const void *a, const void *b) {
    return ( *(int*)a - *(int*)b);
}

uint64_t median(uint64_t* a, uint32_t n) {
    qsort(a, n, sizeof(uint64_t), compare);
    return a[n/2];
}

// this returns the integer mean
uint64_t mean(uint64_t* arr, uint32_t siz) {
  uint64_t sum = 0;
  for (int i=0; i<siz; i++) {
    sum += arr[i];
  }
  return sum/siz;
}

double stdev(uint64_t* arr, double m, uint32_t siz) {
  int var = 0;
  for( int i = 0; i < siz; i++ )
  {
    var += (arr[i] - m) * (arr[i] - m);
  }
  var /= siz;
  return sqrt(var);
}

uint64_t min(uint64_t* arr, uint32_t siz){
  uint64_t _min = LONG_MAX;
  for (int i=0; i<siz; i++) {
    if(arr[i] < _min){
      _min = arr[i];
    }
  }
  return _min;
}

uint64_t max(uint64_t* arr, uint32_t siz){
  uint64_t _max = 0;
  for (int i=0; i<siz; i++) {
    if(arr[i] > _max){
      _max = arr[i];
    }
  }
  return _max;
}