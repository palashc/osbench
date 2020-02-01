#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void proc0() {}
void proc1(int a) {};
void proc2(int a, int b) {};
void proc3(int a, int b, int c) {};
void proc4(int a, int b, int c, int d) {};
void proc5(int a, int b, int c, int d, int e) {};
void proc6(int a, int b, int c, int d, int e, int f) {};
void proc7(int a, int b, int c, int d, int e, int f, int g) {};


/*
 * When inline function pointer is referenced, compiler does not expand inline
 */
void testCall0() { proc0(); }
void testCall1() { proc1(1); }
void testCall2() { proc2(1,2); }
void testCall3() { proc3(1,2,3); }
void testCall4() { proc4(1,2,3,4); }
void testCall5() { proc5(1,2,3,4,5); }
void testCall6() { proc6(1,2,3,4,5,6); }
void testCall7() { proc7(1,2,3,4,5,6,7); }


void testMeasurementOverhead() {}
void testSleep() { sleep(1); }


void testSystemCall(){ getpid(); }

pthread_t tid[1];

void* testThreadf(void * arg){ }
void testKernelThread(){
	pthread_create(&tid[0], NULL, &testThreadf, NULL);
}