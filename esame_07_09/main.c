#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char *argv[])
{

    int K; /* numero di file/processi */

    int pid;       /* pid per fork */
    pipe_t *pipes; /* array di pipe usate a pipeline */

    int k, j;         /* indici */
    int fd, createdF; /* file descriptor */

    int pidFiglio, status, ritorno; /* per valore di ritorno figli */

    char *current; /* array dinamico di linee */

    char ch;    // carattere letto dai figli
    int nr, nw; /* variabili per salvare valori di ritorno di read/write da/su pipe V*/

    int pos; /* variabile per la posizione i caratteri letti */

    /* controllo sul numero di parametri: il quale deve essere strettamente maggiore o uguale di 2 */
    if (argc < 3)
    {
        printf("Errore numero di parametri\n");
        exit(1);
    }

    /* creiamo il file nella directory corrente avente come nome il mio cognome (TUTTO IN MAIUSCOLO, come specificato nel testo) */
    if ((createdF = creat("CARRIERO", PERM)) < 0)
    {
        printf("Errore nella creat del file %s\n", "Carriero");
        exit(2);
    }

    // processi da creare
    K = argc - 1;
    printf("Numero di processi da creare %d\n", K);

    /* allocazione pipe */
    if ((pipes = (pipe_t *)malloc(K * sizeof(pipe_t))) == NULL)
    {
        printf("Errore allocazione pipe\n");
        exit(3);
    }

    /* creazione pipe */
    for (k = 0; k < K; k++)
        if (pipe(pipes[k]) < 0)
        {
            printf("Errore creazione pipe\n");
            exit(4);
        }

    /* allocazione array di caratteri */
    if ((current = malloc(K * sizeof(char))) == NULL)
    {
        printf("Errore allocazione array di linee\n");
        exit(5);
    }

    /* creazione figli */
    for (k = 0; k < K; k++)
    {
        if ((pid = fork()) < 0)
        {
            printf("Errore creazione figlio\n");
            exit(6);
        }
        else if (pid == 0)
        { /* codice figlio */
            printf("Sono il figlio %d e sono associato al file %s\n", getpid(), argv[k + 1]);
            /* nel caso di errore in un figlio decidiamo di ritornare il valore -1 che sara' interpretato dal padre come 255 (valore NON ammissibile) */

            /* chiusura pipes inutilizzate */
            for (j = 0; j < K; j++)
            { /* si veda commento nella definizione dell'array pipes per comprendere le chiusure */
                if (j != k)
                    close(pipes[j][1]);
                if ((k == 0) || (j != k - 1))
                    close(pipes[j][0]);
            }

            /* apertura file */
            if ((fd = open(argv[k + 1], O_RDONLY)) < 0)
            {
                printf("Impossibile aprire il file %s\n", argv[k + 1]);
                exit(7);
            }

            pos = 1;

            /* con un ciclo leggiamo tutti i caretteri*/
            while (read(fd, &ch, 1)>0)
            {
                if (pos % 2 != 0) /* siamo in posizione dispari*/
                {

                    // aspetto di ricevere la struttura dal filgio precedente
                    if (k != 0)
                    {
                        nr = read(pipes[k - 1][0], current, K * sizeof(char));
                        // verifichiamo che la read abbia avuto successo
                        if (nr != K * sizeof(char))
                        {
                            printf("processo ha letto un numero di byte sbagliati %d\n", nr);
                            exit(8);
                        }
                    }

                    // aggiungo all'array il carattere letto
                    current[k] = ch;

                    // invio al figlio successsivo la struttura dati
                    nw = write(pipes[k][1], current, K * sizeof(char));
                    // verifichiamo che la write ha avuto successo
                    if (nw != K * sizeof(char))
                    {
                        printf("Processo ha scritto un numero di byte sbagliati %d\n", nw);
                        exit(9);
                    }

                    // definisco il valore di ritorno
                    ritorno = ch;

                    pos++;
                }
                else
                {
                    // non siamo in posizione dispari
                    pos++;
                }
            }
            /* ogni figlio deve tornare l'ultimo carattere letto */
            exit(ritorno);
        }
    } /* fine for */

    /* codice del padre */
    /* chiusura di tutte le pipe che non usa */
    for (k = 0; k < K; k++)
    {
        close(pipes[k][1]);
        if (k != K - 1)
            close(pipes[k][0]);
    }

    // per ogni array che arriva
    while ((nr = read(pipes[K - 1][0], current, K * sizeof(char))))
    {
        if (nr != K * sizeof(char))
        {
            printf("padre ha letto un numero di byte sbagliati %d\n", nr);
            exit(10);
        }

        for (int i = 0; i < K; i++)
        {
            write(createdF, &current[i], 1);
        }
    }

    /* Il padre aspetta i figli */
    for (k = 0; k < K; k++)
    {
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
        {
            printf("Errore in wait\n");
            exit(11);
        }
        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
        {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %c (se 255 problemi)\n", pidFiglio, ritorno);
        }
    }
    exit(0);
}