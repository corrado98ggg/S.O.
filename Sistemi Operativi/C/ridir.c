#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#define PERM 0644

int copyfile(char *f1, char*f2){
	int infile, outfile, nread;
	char buffer [BUFSIZ]; /* usato per i carattteri*/

close(0);

	if((infile = open(f1, O_RDONLY)) < 0) return 2;
	/*errore se non si riesce ad aprire in lettura il primo file*/
close(1);

	if((outfile = creat(f2, PERM)) < 0)
		/*Errore se non si riesce a creare il secondo file*/
	{close (infile); return 3;}

	while ((nread = read(infile, buffer, BUFSIZ)) > 0){
	if(write(outfile, buffer, nread) < nread){
		/*errore se non si riesce a scrivere*/
		close(infile); close(outfile); return 4;}
}
close(infile); close(outfile); return 0;
/* se arriviamo qui, vuol dire che tutto è andato bene*/
}


int main (int argc, char** argv){
	int status;
	if(argc != 3){ /* controllo sul numero di argoemnti*/
		printf("Errore: numero di argomenti sbagliato");
		printf("Ci vogliono 2 argomenti: nome-del-file e nome-file-destinazione\n");

		exit(1);
	}
status = copyfile( argv[1], argv[2]);

if(status != 0){
	printf("ci sono stati degli errori durante le copia\n");
	return status;
}
}
