#include "syscall.h"

int array[128];

int main(){

	for(int i=0; i<128; i++){
		array[i] = 42;
	}
	Exit(0);
}
