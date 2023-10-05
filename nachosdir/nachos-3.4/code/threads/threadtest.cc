// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
char *debug_char;
Semaphore s(debug_char,0) ;

int SharedVariable,testNum,counter;
void SimpleThread(int which)
{
    int num, val;

//If HW1_SEMAPHORES is defined then proper thread synchronization using 
//semaphores is enabled

    #ifdef HW1_SEMAPHORES
    	s.V();

    	for(num = 0; num<5; num++){
		//This is the start of the critical section
		s.P();
        	val = SharedVariable;
		printf("*** thread %d looped %d times\n", which, num);
		SharedVariable = val+1;
		counter++;
		//This is the end of the critical section
		s.V();
		currentThread->Yield();
    	}
    	while(counter<15)
    	currentThread->Yield();
    	val = SharedVariable;
    	printf("Thread %d sees final value %d\n", which, val);
    
    #endif

//If HW1_SEMAPHORES is not defined then SimpleThread() runs
//without proper thread sychronization

    #ifndef HW1_SEMAPHORES
    	for (num = 0; num < 5; num++) {
     		val = SharedVariable;
      		printf("*** thread %d looped %d times\n", which, num);
      		currentThread->Yield();
      		s.P();
      		testNum+=1;
      		SharedVariable = val+1;	
      		currentThread -> Yield();
    	}
    	val = SharedVariable;
    	printf("Thread %d sees final value %d\n", which, val);
    #endif
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between 'n' threads, by forking 'n' thread 
//	to call SimpleThread. 
//----------------------------------------------------------------------

void
ThreadTest1(int n)
{
    DEBUG('t', "Entering ThreadTest1");
    
    for(int i=0; i <n; i++){
        Thread *t = new Thread("forked thread");
        t->Fork(SimpleThread, i+1);
    }
    SimpleThread(0);

}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest(int numOfThreads)
{
    switch (testnum) {
    case 1:
	    ThreadTest1(numOfThreads);    
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

