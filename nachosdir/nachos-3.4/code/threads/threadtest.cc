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
int threadCounter=1;
int numThreadsActive; // to stop post completion

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

#ifdef HW1_SEMAPHORES
int SharedVariable,testNum,counter;
void SimpleThread(int which)
{
    int num, val;

//If HW1_SEMAPHORES is defined then proper thread synchronization using 
//semaphores is enabled


    	for(num = 0; num<5; num++){
		//This is the start of the critical section
		s.P();
        	val = SharedVariable;
		//printf("*** thread %d looped %d times\n", which, num);
		printf("*** thread %d looped %d times\n", which, val);
		SharedVariable = val+1;
		counter++;
		//This is the end of the critical section
		s.V();
		currentThread->Yield();
    	}
    	while(SharedVariable<(threadCounter*5))
    	currentThread->Yield();
    	val = SharedVariable;
    	printf("Thread %d sees final value %d\n", which, val);
    
    
//If HW1_SEMAPHORES is not defined then SimpleThread() runs
//without proper thread sychronization

    // #ifndef HW1_SEMAPHORES
    // 	for (num = 0; num < 5; num++) {
    //  		val = SharedVariable;
    //   		printf("*** thread %d looped %d times\n", which, num);
    //   		currentThread->Yield();
    //   		s.P();
    //   		testNum+=1;
    //   		SharedVariable = val+1;	
    //   		currentThread -> Yield();
    // 	}
    // 	val = SharedVariable;
    // 	printf("Thread %d sees final value %d\n", which, val);
    // #endif
}
#elif defined(HW1_LOCKS)
int SharedVariable;
Lock * loc;
void SimpleThread(int which) {
    if(loc == NULL)
        {
        loc = new Lock("myloc");
        }

    int num, val;
    for(num = 0; num < 5; num++) { 
    loc->Acquire();
    val = SharedVariable;
    printf("*** thread %d sees value %d\n", which, val);
    currentThread->Yield(); 
    SharedVariable = val+1; 
    loc->Release();
    currentThread->Yield();
    }
    numThreadsActive--;
    while(numThreadsActive!=0){
    currentThread->Yield();
    }
    val = SharedVariable;
    printf("Thread %d sees final value %d\n", which, val);
}

#else
void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

#endif

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between 'n' threads, by forking 'n' thread 
//	to call SimpleThread. 
//----------------------------------------------------------------------
#if defined(HW1_SEMAPHORES)
void
ThreadTest1(int n)
{
    DEBUG('t', "Entering ThreadTest1");
    s.V();
    for(int i=0; i <n; i++){
        Thread *t = new Thread("forked thread");
        t->Fork(SimpleThread, i+1);
		threadCounter++;
    }
    SimpleThread(0);

}
#else

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");
	
    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}
#endif

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------
#if defined(HW1_SEMAPHORES)
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

#elif defined(HW1_LOCKS)

void
ThreadTest(int numOfThreads) {
    DEBUG('t', "Entering SimpleTest");
    Thread *t;
    numThreadsActive = numOfThreads;
    printf("NumthreadsActive = %d\n", numThreadsActive);

    for(int i=1; i<numOfThreads; i++)
    {
        t = new Thread("forked thread");
        t->Fork(SimpleThread,i);
    }
    if(numThreadsActive>0){
    SimpleThread(0);
    }
}

#else

void
ThreadTest(int numOfThreads)
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

#endif



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

