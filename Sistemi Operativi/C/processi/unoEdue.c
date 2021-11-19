#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
	int pid;

	printf("UNO \n");

	if((pid = fork()) < 0){
		/*fork fallita*/
		printf("Errore in fork\n");
		exit(1);
	}

	if(pid==0){
		printf("DUE\n"); /*figlio*/
	} else {
		printf("Ho creato figlio con PID = %d\n", pid); /*padre*/
	}
	exit(0);
}
