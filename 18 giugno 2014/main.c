/* Soluzione della Prova d'esame del 18 Giugno 2014 - Parte C */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char **argv) 
{
	/* -------- Variabili locali ---------- */
	int pid;			/* process identifier per le fork() */
	int N; 				/* numero di file passati sulla riga di comando (uguale al numero di file) */
	int X; 				/* numero passato come ultimo parametro e che rappresenta la lunghezza in linee dei file passati sulla riga di comando */
	char b[15];			/* buffer per i nomi dei file da creare in /tmp: basterebbero 9 char 5 caratteri per /tmp/ e 3 caratteri al massimo per arrivare fino al valore massimo di X (dalla parte Shell di vede che X e' strettamente positivo e strettamente minore di 255) e quindi il terminatore di stringa: abbiamo aggiunto altri 6 char perche' abbiamo deciso di aggiungere nel nome, prima del numero richiesto, anche la stringa corrispondente al PID del padre con anche un trattino '-' altrimenti chiamando la parte C piu' volte i file vengono riazzerati e non si riesce a controllare il funzionamento! */
	int *fdout; 			/* array dinamico di file descriptor per creazione file da parte del padre */
	int fd; 			/* file descriptor per apertura file */
	int status;			/* variabile di stato per la wait */
	pipe_t *piped;			/* array dinamico di pipe descriptors per comunicazioni figli-padre */
	int i, j;			/* indici per i cicli */
	char linea[512];		/* array di caratteri per memorizzare la linea, supponendo una lunghezza massima di ogni linea di 512 caratteri compreso il terminatore di linea */
	int valore; 			/* variabile che viene usata dal padre per recuperare il valore comunicato da ogni figlio e che contiene la lunghezza della linea corrente */
	int somma = 0; 			/* variabile usata dal figlio per calcolare la lunghezza del file per poi calcolare la lunghezza media */
	int ritorno; 			/* variabile che viene ritornata da ogni figlio al padre e che contiene la lunghezza media delle linee (supposta strettamente minore di 255) */
	/* ------------------------------------ */
	printf("Sono il padre con pid %d\n", getpid());
	
	/* Controllo sul numero di parametri */
	if (argc < 3) /* Meno di due parametri */  
	{
		printf("Errore nel numero dei parametri\n");
		exit(1);
	}

	/* Calcoliamo il numero di file passati */
	N = argc - 2;
	/* convertiamo l'ultima stringa in un numero */
	X = atoi(argv[argc-1]);
	
	/* allochiamo la memoria per l'array di file descriptor dei file che creeremo */
	fdout = (int *) malloc(X * sizeof(int));
	if (fdout == NULL)
	{
		printf("Errore nella allocazione della memoria\n");
		exit(2);
	}

	/* creazione file in /tmp */
	for (j=1; j <= X; j++)
	{
	/* come indicato nel commento sulla variabile b, aggiungiamo il PID del processo padre e un trattino */
	sprintf(b,"/tmp/%d-%d", getpid(), j);
	if ((fdout[j-1]=creat(b, 0644)) < 0)  /* dato che l'indice del for parte da 1 dobbiamo decrementarlo per usarlo come indice dell'array */ 
			{
                                printf("Errore nella creat del file %s\n", b);
                                exit(3);
                       	}
	}
	
	/* Allocazione dell'array di N pipe descriptors*/
	piped = (pipe_t *) malloc (N*sizeof(pipe_t));
	if (piped == NULL)
	{
		printf("Errore nella allocazione della memoria\n");
		exit(4);
	}
	
	/* Creazione delle N pipe figli-padre */
	for (i=0; i < N; i++)
	{
		if(pipe(piped[i]) < 0)
		{
			printf("Errore nella creazione della pipe\n");
			exit(5);
		}
	}
	
/* Ciclo di generazione dei figli */
	for (i=0; i < N; i++)
	{
		if ( (pid = fork()) < 0)
		{
			printf("Errore nella fork\n");
			exit(6);
		}
		
		if (pid == 0) 
		{
			/* codice del figlio */
			printf("Sono il processo figlio di indice %d e pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);
			/* Chiusura delle pipe non usate nella comunicazione con il padre */
			for (j=0; j < N; j++)
			{
				close(piped[j][0]);
				if (i != j) close(piped[j][1]);
			}

			if ((fd=open(argv[i+1], O_RDONLY)) < 0)
			{
                                printf("Errore nella open del file %s\n", argv[i+1]);
                                exit(-1); /* in caso di errore nei figli decidiamo di tornare -1 che corrispondera' per il padre al valore 255 che supponiamo non essere un valore accettabile di ritorno */
                       	}
			/* adesso il figlio legge dal file una linea alla volta */
			j=0;
		        while (read(fd, &(linea[j]), 1))
			{
				if (linea[j] == '\n') 
			 	{ /* dobbiamo mandare al padre la lunghezza della linea corrente compreso il terminatore di linea (come int) e quindi incrementiamo j */
				   j++;
				   write(piped[i][1], &j, sizeof(j));
				   /* e quindi la linea stessa */
				   write(piped[i][1], linea, j);
				   /* aggiorniamo il conteggio dei caratteri per calcolare alla fine la lunghezza media delle linee */
				   somma=somma+j;
				   j=0; /* azzeriamo l'indice per le prossime linee */
				}
				else j++; /* continuiamo a leggere */
			}
			/* il figlio Pi deve ritornare al padre il valore corrispondente alla lunghezza media delle linee del proprio file associato */
			ritorno=somma/X;
			exit(ritorno);
  			}	
		}
	
/* Codice del padre */
/* Il padre chiude i lati delle pipe che non usa */
	for (i=0; i < N; i++)
		close(piped[i][1]);

/* Il padre recupera le informazioni dai figli: prima in ordine di linee e quindi in ordine di indice */
	for (j=1; j <= X; j++)
		for (i=0; i < N; i++)
		{ 
		/* il padre recupera tutti i valori interi dai figli */
		read(piped[i][0], &valore, sizeof(valore));
		read(piped[i][0], linea, valore);
		/* adesso va scritta sul file giusto */
		write(fdout[j-1], linea, valore);
		}	

	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
	{
		pid = wait(&status);
		if (pid < 0)
		{
		printf("Errore in wait\n");
		exit(7);
		}

		if ((status & 0xFF) != 0)
    		printf("Figlio con pid %d terminato in modo anomalo\n", pid);
    		else
		{ ritorno=(int)((status >> 8) &	0xFF); 
		  printf("Il figlio con pid=%d ha ritornato %d (se 255 significa che il figlio e' terminato con un errore)\n", pid, ritorno);
		}
	}
exit(0);
}