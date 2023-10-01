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

int SharedVariable;
void SimpleThread(int which)
{
    int num, val;

    for (num = 0; num < 5; num++) {
        val = SharedVariable;
	    printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
        SharedVariable = val+1;
        currentThread -> Yield();
    }
    val = SharedVariable;
    printf("Thread %d sees final value %d\n", which, val);
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

