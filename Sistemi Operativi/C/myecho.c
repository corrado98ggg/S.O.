#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	printf("sono myecho\n");

	for (int i = 0; i < argc; i++){
		printf("argoemnto argv[%d]= %s\n", i, argv[i]);
		exit(0);
	}

}
