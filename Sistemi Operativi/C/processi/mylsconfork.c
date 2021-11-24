#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){


int pid;
int pidfiglio, status, ritorno;

pid=fork();

if(pid < 0){

	printf("errore in fork");
	exit(1);
}

if (pid==0){
 printf("Esecuzione di ls da parte del figlio con pid %d\n", getpid());
 execlp("ls", "ls", "-l", (char *)0);

 exit(-1);

}

pidfiglio= wait(&status);

if(pidfiglio < 0){
	printf("errore wait\n");
	exit(2);
}

if((status & 0xFF) != 0){
	printf("figlio con pid %d, terminato in modo anomalo\n", pidfiglio);
} else{

	ritorno=(int) ((status >> 8) & 0xFF);
	printf("il figlio con pid=%d ha ritornato %d", pidfiglio, ritorno);
}
exit(0);

}
