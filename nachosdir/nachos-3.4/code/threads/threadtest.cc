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

#elif defined(HW1_ELEVATOR)
//Set up for the threads
struct PersonThread {
    int id;  //Each person is assigned a unique number
    int atFloor; //Marks what floor they are currently at
    int toFloor; //Marks what floor they want to go to
};

struct ElevatorThread {
    int numFloors; //Marks the number of floors in building
    int currentFloor; //Marks the current floor
    int numPeopleIn; //Max 5 people in elevator at 1 time
};

void Elevator(int numFloors) {
    //Starts up the elevator; there will always be at least 1 floor and the first floor is labeled 1
    ElevatorThread elevator;
    //Putting the number of floors into the struct so it can be accessed by the ArrivingGoingFromTo function
    elevator.numFloors = numFloors;

    //When the elevator is started, it should start on floor 1
    elevator.currentFloor = 1;

    //When the elevator is started, there should be no one inside of the elevator
    elevator.numPeopleIn = 0;

    //This is going to need a way to keep track of the number of people present in the elevator at a time -- counting semaphore to be used
    //To use a semaphore to keep track of the number of people in the elevator and block off any requests when the elevator is full, the semaphore will be
    //incremented 5 times to give it a value of 5. Each person that boards the elevator decrements the semaphore by 1 and each person that was on the elevator that gets off
    //increments the semaphore by 1.
    for (int i = 0; i < 5; i++) {
        s.V();
    }
}

void ArrivingGoingFromTo(int atFloor, int toFloor) {
    //if Elevator was not called before, then this function wouldn't work because the elevator isn't working.
    //As long as the counting semaphore has more than 0 as a value, then the elevator is active.
    //When an elevator request is made
    PersonThread person;
    ElevatorThread elevator;

    //First thing is that the requestor is assigned an unique id - composed of atFloor number, toFloor number, and numPeopleIn + 1
    person.id = (atFloor*100) + (toFloor*10) + (elevator.numPeopleIn + 1);
    //Then the rest of the information is added to the struct
    person.atFloor = atFloor;
    person.toFloor = toFloor;
    //The first statement is printed
    printf("Person %d wants to go to floor %d from floor %d.", person.id, person.atFloor, person.toFloor);

    //For getting on the elevator
    int floor_differenceA = 0;
    while (elevator.currentFloor != person.atFloor) {
        //If the elevator is not on the right floor, then the elevator has to travel to the nearest requestor in accordance to the
        //direction that the earliest current requestor requested
        if (elevator.currentFloor < person.atFloor) {
            //Case of elevator being on the lower floor than earliest current requestor
            floor_differenceA = person.atFloor - elevator.currentFloor;
            for(int i = 0; i < floor_differenceA; i++) {
                //Movement between each floor time -- 50 ticks
                for (int aFloortime = 0; aFloortime < 50; aFloortime++) {
                    //There doesn't need to be anything in here since we're mostly after the for loop activating each time the elevator moves between floors.
                }
                elevator.currentFloor++;
                printf("Elevator arrives on floor %d.", elevator.currentFloor);
                //Checks if there is someone waiting on this floor
                currentThread->Yield();
            }
        }
        else {
            //Case of elevator being on the higher floor than earliest current requestor
            floor_differenceA = elevator.currentFloor - person.atFloor;
            for(int i = 0; i < floor_differenceA; i++) {
                //Movement between each floor time -- 50 ticks
                for (int aFloortime = 0; aFloortime < 50; aFloortime++) {
                    //There doesn't need to be anything in here since we're mostly after the for loop activating each time the elevator moves between floors.
                }
                elevator.currentFloor--;
                printf("Elevator arrives on floor %d.", elevator.currentFloor);
                //Checks if there is someone waiting on this floor
                currentThread->Yield();
            }
        }
    }

    //In the case where the elevator is on the right floor -- into elevator
    if (elevator.currentFloor == person.atFloor) {
        //Critical Section
        s.P();
        elevator.numPeopleIn++;
        printf("Person %d got into the elevator.", person.id);
    }

    //For getting off of the elevator
    int floor_differenceD = 0;
    while (elevator.currentFloor != person.toFloor) {
        //If the elevator is not on the right floor, it needs to take the passengers to the right floors.
        if (elevator.currentFloor < person.toFloor) {
            //The elevator needs to go up
            floor_differenceD = person.toFloor - elevator.currentFloor;
            for (int i = 0; i < floor_differenceD; i++) {
                //Movement between each floor time -- 50 ticks
                for (int aFloortime = 0; aFloortime < 50; aFloortime++) {
                    //There doesn't need to be anything in here since we're mostly after the for loop activating each time the elevator moves between floors.
                }
                elevator.currentFloor++;
                printf("Elevator arrives on floor %d.", elevator.currentFloor);
                //Checks if there is someone waiting on this floor
                currentThread->Yield();
            }
        }
        else {
            //The elevator needs to go down
            floor_differenceD = elevator.currentFloor - person.toFloor;
            for(int i = 0; i < floor_differenceD; i++) {
                //Movement between each floor time -- 50 ticks
                for (int aFloortime = 0; aFloortime < 50; aFloortime++) {
                    //There doesn't need to be anything in here since we're mostly after the for loop activating each time the elevator moves between floors.
                }
                elevator.currentFloor--;
                printf("Elevator arrives on floor %d.", elevator.currentFloor);
                //Checks if there is someone waiting on this floor
                currentThread->Yield();
            }
        }
    }

    //In the case where the elevator is on the right floor -- out of elevator
    //The conditionals prevent numPeopleIn from becoming negative
    if ((elevator.currentFloor == person.toFloor) && (elevator.numPeopleIn >= 1)) {
        s.V();
        elevator.numPeopleIn--;
        printf("Person %d got out of the elevator.", person.id);
    }
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
