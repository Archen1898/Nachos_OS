#include "syscall.h"

void useMemory(){
	Exec("../test/memory");
}

int main(){

	for(int i =0; i< 5; i++){
		//Fork(useMemory);
		useMemory();
		Yield();
	}
	Exit(0);
}
