#include "syscall.h"

void useMemory(){
	Exec("../test/memory");
}

int main(){
	int i;
	for(i =0; i< 5; i++){
		Fork(useMemory);
		useMemory();
		Yield();
	}
	Exit(0);
}
