#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#define PERM 0644

int main(){

	int pid;
	int pidfiglio, status, ritorno;

	pid = fork();

	if(pid < 0){
		printf("errore\n");
		exit(1);
	}

	if(pid==0){
		printf("real-user id = %d\n", getuid());
		printf("effective-user id = %d\n", getuid());
		printf("esecuzione di programma (con suid settato) che visualizza file (leggibile solo da proprietariio)\n");
		execl("LeggiPippo1", "leggiPippo1", (char*) 0);
		exit(-1);

		pidfiglio = wait(&status);

		if(pidfiglio < 0){
			printf("figlio con pid %d terminato in modo anomalo\n", pidfiglio);

		}else{
			ritorno = (int) ((status >> 8) & 0xFF);
	
			printf("il figlio con pid=%d ha ritornato %d\n", pidfiglio, ritorno);
		}
		exit(0);
}
