#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char* argv[]){

	int pid;
	int status;
	pipe_t *piped; //array dinamico di pipe descriptors per comunicazioni figli-padre
	int j, k;
	int lunghezza;
	int ritorno;



	if(argc < 3 ){
		printf("Errore nel numero dei parametri\n");
		exit(1);
	}

	M = argc -1;

	if(piped == NULL){
		printf("errore nell'allocazione della memoria\n");
		exit(2);
	}

	for(j =0; j < M;j++){
	
	
		if(pipe(piped[j]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}

		printf("sono il processo padre con pid %d e sto per generare %d figli\n", getpid(), M);


		for(j = 0; j < M; j++){

			if((pid = fork()) < 0){
				printf("errore nella fork\n");
				exit(4);
			}

			if(pid == 0){
			
			printf("sono il processo di indice %d e pid %d sto per creare il nipote che recuperarerà l'ultima linea del file %s\n", getpid(), argv[j+1]);

			for(k=0; k<M;k++){
				close(piped[k][0];
				if(k!=j) close (piped[k][1]);
			
			}

	lunghezza = 3000+j;
	write(piped[j][1], &lunghezza, sizeof(lunghezza));
        exit(0);
	}
	}

	}

for (j = M -1; j >= 0; j--){
close(piped[j][1]);
}

for (j = M - 1; j >= 0; j--){
read(piped[j][0], &lunghezza, sizeof(lunghezza));
printf("il figlio di indice %d ha comunicato il valore %d per il file %s\n", j, lunghezza, argv[j+1]);

}


for (j =0; j < M; j++){

	pid = wait(&status);
	if(pid < 0){
	printf("errore in wait\n");
	exit(5);
	}


	if((status & 0xFF) != 0){
		printf("figlio con pid %d terminato in modo anomalo\n", pid);
	} else {
	 ritorno = (int)((status >> 8) & 0xFF);

	if(ritorno != 0)
		printf("il figlio con pid %d ha ritornato %d e quindi vuole dire che il nipote non e' riuscito ad eseguire il tail oppure il figlio o il nipote sono incorsi in errori\n", pid, ritorno);
	else printf("il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
	}
}
exit(0);
}
