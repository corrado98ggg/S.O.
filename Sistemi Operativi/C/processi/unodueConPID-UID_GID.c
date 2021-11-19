#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){

	printf("UNO. PID = %d, UID = %d, GI = %d\n", getpid(), getuid(), getgid());

	fork();

	printf("DUE. PID = %d, UID = %d, GI = %d\n", getpid(), getuid(), getgid());
	exit(0);
}

