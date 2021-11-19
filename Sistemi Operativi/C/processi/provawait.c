#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

	int pid, pidFiglio; /*pid per fork e pidFiglio per wait*/

	if((pid = fork()) < 0){

		printf("Errore in fork\n");
		exit(1);
	}

	if(pid == 0){
		printf("Esecuzione del figlio\n");
		sleep(4); /* si simula un ritardo di 4 secondi che il figlio faccia qualcosa*/
		exit(5); /* si torna un valore che si suppone possa essere derivante dell'esecuzione di un compito assegnato al figlio*/
	}


	/*padre*/

	printf("Generato figlio con PId = %d", pid);
	/* il padre aspetta il figlio del valore della exit del figlio*/
	if((pidFiglio=wait((int *)0)) < 0){
		printf("errore in wait\n");
		exit(2);
	}

	else{
		/*problemi*/
		printf("il pid della wait non corrisponde al pid della fork\n");
		exit(3);
	}
	exit(0);
}

