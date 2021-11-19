#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("UNO\n");

	fork();
	printf("DUE\n");
	exit(0);
}
