#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	char *av[3]; //array di puntatori a char che serve per passare i param alla execv

	av[0]="ls"; /*puntatore a char cui viene assegnato il puntatore alla stringa "ls" */
	av[1]="-l";
	av[2]=(char *)0; //viene assegnato il valore 0 come puntatore a char

	printf("esecuzione di ls:prima versione\n");
	execv("/bin/ls", av);

	//si esegue l'istruzione seguente solo in caso di fallimento della exec
	printf("errore in execv\n");
	exit(1);
}
