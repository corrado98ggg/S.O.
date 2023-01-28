#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* definiamo un tipo per un array di due interi */
typedef int pipe_t[2];
/* creiamo questa struttura in modo che il padre salvi oltre alla lunghezza della linea anche l'indice del figlio che l'ha mandata */
/* NOTA BENE: NON VIENE USATA QUESTA STRUTTURA PER LA COMUNICAZIONE! */
typedef struct{
   int len;
   int indice;
}strutt;

void bubbleSort(strutt v[],int dim) /* il tipo degli elementi NON e' un semplice int come riportato sul testo, ma deve essere il tipo strutt (appositamente definito) */
{
int i;
int ordinato=0;
strutt a;  /* variabile di appoggio per fare lo scambio */
	while (dim>1 && ordinato!=1)
	{
   		ordinato=1;
   		for (i=0;i<dim-1;i++)
      		if (v[i].len > v[i+1].len) /* chiaramente il confronto va fatto sul campo len della struttura */
      		{  /* viene effettuato in questo caso un ordinamento in senso crescente */
         		/* scambio gli elementi */
         		a=v[i];
         		v[i]=v[i+1];
         		v[i+1]=a;
         		ordinato=0;
      		}
   		dim--;
	}	
}/* fine bubblesort */

int main(int argc,char **argv)
{
int pid, pidFiglio; 	/* pid per le fork e la wait*/
int N;  		/* numero di file passati come parametri e numero di processi figli da creare */
int K;  		/* intero che rappresenta l'ultimo parametro passato */
int X;  		/* valore intero che deve essere inserito da tastiera */
int L;  		/* lunghezza di ogni linea calcolata dai figli */
int status, ritorno;	/* variabili di stato per le wait */
int i,j;   		/* indici per cicli */
int retval; 		/* variabile di ritorno dei figli */
char linea[512]; 	/* array che serve per salvare la linea letta da ogni figlio, supposta non piu' lunga di 512 caratteri compreso il terminatore di linea */
int fd;  		/* generico file descriptor */
strutt *vettore; 	/* vettore dinamico dove il padre salva le lunghezze ricevute dagli N figli ed i loro indici */
pipe_t *pfds_p;   	/* vettore dinamico per pipe padre -> figlio */
pipe_t *pfds_f;   	/* vettore dinamico per pipe figlio -> padre */
			/* N.B. servono 2*N pipe */

/* controlliamo il numero di parametri */
/* devono esserci almeno due parametri più argv[0] */
if (argc < 3)
{
   printf("Errore nel numero di parametri: %d\n", argc);
   exit(1);
}

/* calcoliamo il numero di file passati */
N=argc-2;

/* calcoliamo valore di K e lo controlliamo */
K=atoi(argv[argc-1]);
if (K < 0)
{
   printf("Errore valore di K\n");
   exit(2);
}
/* allocazione dinamica del vettore di lunghezze prima descritto */
vettore=(strutt *)malloc(sizeof(strutt)*N);
if (vettore == NULL)
{
   printf("allocazione fallita vettore delle lunghezze\n");
   exit(3);
} 
/* allocazione del vettore dinamico per le pipe padre-figli */
pfds_p=(pipe_t *)malloc(sizeof(pipe_t)*N);
if (pfds_p == NULL)
{
   printf("Allocazione fallita pipe padre-figli\n");
   exit(4);
}
/* allocazione dinamica pipe figli-padre */
pfds_f=(pipe_t *)malloc(sizeof(pipe_t)*N);
if (pfds_f == NULL)
{
   printf("Allocazione fallita pipe figli-padre\n");
   exit(5);
}
/* creazione delle 2 * N pipe */
for (i=0;i<N;i++)
{
   if (pipe(pfds_p[i])!=0 || pipe(pfds_f[i])!=0)
   {
      printf("Errore pipe\n");
      exit(6);
   }
}
/* chiediamo all'utente di inserire un numero X da tastiera */
printf("Inserire un numero intero strettamente positivo X e minore o uguale a %d\n",K);
scanf("%d",&X); /* lo leggiamo con la scanf che e' la cosa piu' semplice */
/* controlliamo X */
if (X<=0 || X>K)
{
   printf("Il valore X inserito non e' valido\n");
   exit(7);
}

/* ciclo per la generazione degli N figli */
for (i=0;i<N;i++)
{
   pid=fork();
   if (pid < 0)  /* errore */
   {
      printf("Errore fork\n");
      exit(8);
   }
   if (pid == 0)  /* figlio i-esimo */
   {
      /* codice del figlio */
      /* stampa di debugging */
      printf("Figlio di indice %d e pid %d: selezionero' la linea %d-esima dal file %s\n",i,getpid(), X, argv[i+1]);
      /* chiudo le pipe che non servono */
      /* ogni figlio legge solo da pfds_p e scrive solo su pfds_f */
      for (j=0;j<N;j++)
      {
         close(pfds_p[j][1]);
         close(pfds_f[j][0]);
         if(j!=i)
         {
            close(pfds_p[j][0]);
            close(pfds_f[j][1]);
         }
      }

      /* apro il file associatoa, ossia quello di indice i+1 */
      fd=open(argv[i+1],O_RDONLY);
      if (fd < 0)
      {
         printf("Errore apertura file. Sono il figlio di indice %d\n",i);
         exit(-1); /* in caso di errore decidiamo di tornare -1 che sara' interpretato come 255 */
      }

      /* con un ciclo leggo la linea X-esima e ne calcolo la lunghezza */
      L=0; /* valore iniziale dell'indice */
      j=1; /* inizializzo il conteggio delle linee a 1 */
      while(read(fd,&(linea[L]),1) != 0)
      {
		if (linea[L] == '\n')
        	{
                	if (j == X) /* trovata la linea che dobbiamo selezionare */
			{
				/* printf("STAMPA DI DEBUGGING: La linea numero %d del file %s e':\n%s", j, argv[i+1], linea); */
                        	break; /* usciamo dal ciclo di lettura */
 			}			
                	else
                	{       L = 0; /* azzeriamo l'indice per la prossima linea */
                        	j++;    /* se troviamo un terminatore di linea incrementiamo il conteggio */
                	}
        	}	
        	else L++;
      }	
      L++; /* incrementiamo L per tenere conto anche del terminatore di linea */
      /* comunichiamo L al processo padre */
      write(pfds_f[i][1],&L,sizeof(L));
  
      /* si deve aspettare la comunicazione del padre prima di stampare la linea X-esima su stdout */
      read(pfds_p[i][0],&L,sizeof(L)); /* dato che questa read e' solo di sincronizzazione il padre puo' mandare qualunque valore: decidiamo che il padre, reinvii indietro la lunghezza della linea */
      write(1, linea, L);

      retval=(L%255); /* il valore di ritorno dei figli deve essere il resto della divisione intera di L con 255 (sicuramente un valore minore di 255) */
      exit(retval);

      /* fine codice del figlio */
   }
}
/* codice del padre */
/* chiudo le pipe che il padre non usa */
for (i=0;i<N;i++)
{
   close(pfds_f[i][1]);
   close(pfds_p[i][0]);
}
/* riceviamo dai figli le lunghezze calcolate, in ordine di indice */
for (i=0;i<N;i++)
{
   read(pfds_f[i][0],&(vettore[i].len),sizeof(int));
   /* printf("STAMPA DI DEBUGGING: padre letto lunghezza %d dal figlio di indice %d\n", vettore[i].len, i);  */
   vettore[i].indice=i; /* inseriamo anche l'indice del processo figlio da cui e' stata ricevuta la lunghezza */
}

/* printf("STAMPA DI DEBUGGING: padre sta per ordinare il vettore\n"); */

/* ordiniamo il vettore in modo crescente */
bubbleSort(vettore,N);
/* printf("STAMPA DI DEBUGGING: padre ha ordinato il vettore\n");*/

/* comunichiamo ai figli di stampare la linea X-esima */
/* scorro il vettore ordinato e comunico con il figlio: deciso di comunicare di nuovo la lunghezza al figlio per fargli capire che può procedere */
/* scrivo sulla pipe corrispondente all'indice salvato nel vettore */
for (i=0;i<N;i++)
{  
   /* printf("STAMPA DI DEBUGGING: Nome file: %s, lunghezza: %d\n",argv[(vettore[i].indice+1)],vettore[i].len); */
   write(pfds_p[vettore[i].indice][1],&(vettore[i].len),sizeof(int));
   sleep(1); /* introduciamo un leggero ritardo in modo da avere la stampa corretta da parte dei figli */
}

/* Attesa della terminazione dei figli */
for (i=0;i<N;i++)
{
   pidFiglio = wait(&status);
   if (pidFiglio < 0)
   {
      printf("Errore wait\n");
      exit(9);
   }
   if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
   else
   {
                ritorno=(int)((status >> 8) & 0xFF);
                printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio, ritorno);
   }
}
exit(0);
}/* fine del main */

