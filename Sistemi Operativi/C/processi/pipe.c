#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#define MSGSIZE 5

int main(int argc, char* argv[]){
	int pid, j, piped[2];
	char mess[MSGSIZE];
	char inpbuf[MSGSIZE];
	int pidfiglio, status, ritorno;

	if(argc != 2){
		printf("numero dei param errato %d;: ci vuuole un singolo param\n", argc);
		exit(1);
	}

	if(pipe(piped) < 0){
		printf("errore creazione pipe\n");
		exit(2);
	}

	 if((pid = fork()) < 0){
                printf("errore creazione figlio\n");
                exit(3);
        }

	 if(pid == 0){
		 int fd;
		 close(piped[0]);
		 if((fd = open(argv[1], O_RDONLY)) < 0){
			 printf("errore in apertura file %s\n", argv[1]);
			 exit(-1);
		 }

		 printf("figlio %d sta per iniziare a scrivere una serie di messaggi, ognuno di lunghezzza %d, sulla pipe dopo averli letti dal file passato come parametro\n", getpid(), MSGSIZE);

		 while(read(fd, mess, MSGSIZE)){
		 
			 mess[MSGSIZE-1]='\0';
			 write(piped[1], mess, MSGSIZE);
			 j++;
		 }

		 printf("figlio %d ha scritto %d messaggi sulla pipe\n", getpid(), j);
	 }
//padre


close(piped[1]);

printf("padre %d sta per iniziare a leggere i messaggi della pipe\n", getpid());j = 0;
while(read(piped[0], inpbuf, MSGSIZE)){
	printf("%d: %s\n", j, inpbuf);
	j++;
}
	printf("padre %d ketto %d messaggi dalla pipe\n", getpid(), j);
	pidfiglio= wait(&status);

	if(pidfiglio < 0){
		printf("errore wait\n");
		exit(4);
	}

	 if(pipe(piped) < 0){
                printf("figlio con pid %d terminato in modo anomalo\n", pidfiglio);
        } else {
	ritorno = (int) ((status >> 8) & 0xFF);
	printf("Il figlio con pid=%d ha ritornato %d\n", pidfiglio, ritorno);

	}

	 exit(0);

}
