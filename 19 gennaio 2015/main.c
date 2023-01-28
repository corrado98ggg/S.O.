/* Soluzione della parte C del compito della 19 Gennaio 2015 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef int pipe_t[2];
typedef pipe_t * p_t; 		/* definiamo anche un tipo puntatore a pipe_t per semplificare la definizione di un array di due elementi, ognuno dei quali e' un array di pipe */
typedef struct{
        int pid; 		/* pid figlio */
	long int occmax; 	/* numero massimo di occorrenze */
	long int occtotale; 	/* somma del numero di occorrenze */
		} s_occ;

int main (int argc, char **argv)
{
int P, N; 		/* P numero di file che sara' uguale a 2N */
int pid;		/* pid per fork */
p_t pipes[2];		/* array di array di pipe usate a pipeline da ultimo figlio, a penultimo figlio .... primo figlio e poi a padre: ogni processo (a parte l'ultimo) legge dalla pipe i+1 e scrive sulla pipe i */
int i,j,k,h; 		/* contatori */
int fd; 		/* file descriptor */
int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
char C[2], ch; 		/* array con i due caratteri da cercare e carattere letto da file */
long int cur_occ; 	/* conteggio occorrenze per figlio */
s_occ letta;            /* struttura usata dai figli e dal padre */
int nr,nw;              /* variabili per salvare valori di ritorno di read/write da/su pipe */


/* controllo sul numero di parametri almeno 2 file e due caratteri */
if (argc < 5)
{
	printf("Errore numero di parametri\n");
	exit(1);
}
/* Calcoliamo il numero di file passati */
P = argc - 3;
if (P % 2) /* Non pari */
{
	printf("Errore nel numero dei parametri: non pari\n");
        exit(2);
}
/* Calcoliamo N */
N = P/2;

/* controlliamo che gli ultimi due paramentri siano singoli caratteri */
if (strlen(argv[argc-2]) != 1)
{
	printf("Errore penultimo parametro non singolo carattere\n");
	exit(3);
}
else C[0] = argv[argc-2][0];

if (strlen(argv[argc-1]) != 1)
{
	printf("Errore ultimo parametro non singolo carattere\n");
	exit(4);
}
else C[1] = argv[argc-1][0];
 
printf("Numero di processi da creare %d con N %d e C1 %c e C2 %c\n", P, N, C[0], C[1]);

/* allocazione pipe: dobbiamo allocare i due array di pipe, quello di indice 0 servira' per i primi N processi e quello di indice 1 servira' per i secondi N processi */
for (j=0; j < 2; j++)
if ((pipes[j]=(p_t)malloc(N*sizeof(pipe_t))) == NULL)
{
	printf("Errore allocazione pipei per j = %d\n", j);
	exit(5); 
}

/* creazione pipe: dobbiamo creare N + N pipe */
for (j=0; j < 2; j++)
	for (i=0;i<N;i++)
		if(pipe(pipes[j][i])<0)
		{
			printf("Errore creazione pipe\n");
			exit(6);
		}

/* creazione figli: dobbiamo creare N + N processi */
for (j=0; j < 2; j++)
	for (i=0;i<N;i++)
	{
		if ((pid=fork())<0)
		{
			printf("Errore creazione figli\n");
			exit(7);
		}	
	if (pid==0)
	{ /* codice figlio */
	printf("Sono il figlio %d con file %s e car %c\n", getpid(), argv[i+1+j*N], C[j]);
	/* dato che il testo non lo specifica, decidiamo di tornare al padre il valore 0 in caso di successo, mentre in caso di errore decidiamo di ritornare un valore via via crescente negativo */
	/* chiusura pipes inutilizzate */

	for (h=0; h < 2; h++)
		for (k=0;k<N;k++)
		{
		if ( k != i && k !=(i+1))
			{
				close(pipes[h][k][0]);		/* lato lettura */
				close(pipes[h][k][1]);		/* lato scrittura */
			}
		}
	/* non devo leggere dalla pipe i-esima */
		close(pipes[h][i][0]);	
        /* non devo scrivere sulla pipe i+1: usiamo l'operatore % con valore N in modo che per il figlio N-1esimo si vada a chiudere il lato scrittura della pipe p[0]  */
                close(pipes[h][(i+1)%N][1]);

	/* inizializziamo il contatore di occorrenze */
	cur_occ= 0;
	
	/* apertura file */
	if ((fd=open(argv[i+1+j*N],O_RDONLY))<0)
	{
		printf("Impossibile aprire il file %s\n", argv[i+1+j*N]);
		exit(-1);
	}
	while(read(fd,&ch,1)>0)
	{
		/* cerco il carattere */
		if (ch == C[j])
	 	{
		cur_occ++;
   		}	
	}
	if (i == N-1)
	{  /* il figlio di indice N-1 deve preparare la struttura da mandare al figlio seguente */
	letta.pid = getpid();
	letta.occmax = cur_occ;
	letta.occtotale = cur_occ; 
    	}
	else
 	{	nr=read(pipes[j][i+1][0],&letta,sizeof(s_occ));
		/* lettura da pipe della struttura per tutti i figli a parte l'ultimo */
		if (nr != sizeof(s_occ))
        	{	
                	printf("Figlio %d ha letto un numero di byte sbagliati %d\n", i, nr);
                	exit(-2);
        	}
		if (letta.occmax < cur_occ)
	        {  /* il figlio di indice i ha calcolato un numero di occorrenze maggiore e quindi vanno aggiornati i valori di letta */
        	letta.pid = getpid();
        	letta.occmax = cur_occ;
        	}	
        	/* il valore della somma, va aggiornato comunque */
		letta.occtotale+= cur_occ;
	}
	 
	/* tutti i figli mandano indietro, il primo figlio manda al padre una struttura */
        nw=write(pipes[j][i][1],&letta,sizeof(s_occ));
        if (nw != sizeof(s_occ))
        {
                printf("Figlio %d ha scritto un numero di byte sbagliati %d\n", i, nw);
                exit(-3);
        }
	/* deciso di tornare zero in caso di successo */
	exit(0);
	}
} /* fine for */

/* codice del padre */
/* chiusura pipe: tutte meno l'ultima in lettura */
for (j=0; j < 2; j++)
for(i=0;i<N;i++)
	{
	close (pipes[j][i][1]);
	if (i != 0) close (pipes[j][i][0]);
	}

for (j=0; j < 2; j++)
{
/* il padre deve leggere la struttura che gli arriva dai primi figli delle due serie */
nr=read(pipes[j][0][0],&letta,sizeof(s_occ));
if (nr != sizeof(s_occ))
{
        printf("Padre ha letto un numero di byte sbagliati %d\n", nr);
        exit(8);
}
/* il padre deve stampare i campi della struttura ricevuta */
printf("Il figlio della serie %d con pid %d ha trovato il numero massimo di occorrenze %ld del carattere %c\n", j+1, letta.pid, letta.occmax, C[j]);
printf("I figli della serie %d hanno trovato in totale %ld occorrenze del carattere %c nei file\n", j+1, letta.occtotale, C[j]);
}
/* Il padre aspetta i figli */
for (j=0; j < 2; j++)
for (i=0; i < N; i++)
{
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
	{
                printf("Errore in wait\n");
                exit(9);
        }

        if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
        { 	ritorno=(int)((status >> 8) & 0xFF);
                printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio, ritorno);
        }
}
exit(0);
}