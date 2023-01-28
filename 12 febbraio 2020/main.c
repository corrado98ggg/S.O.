/* Soluzione della parte C del compito del 12 Febbraio 2020 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef int pipe_t[2];
typedef struct
{
    long int primo_conteggio;   /* pid figlio (campo c1 del testo) */
    long int secondo_conteggio; /* numero occorrenze (campo c2 del testo) */
} struttura;

int main(int argc, char **argv)
{
    int N;                          /* numero di file (NOME STABILITO DAL TESTO!) */
    int pid;                        /* pid per fork */
    pipe_t *pipes;                  /* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo (a parte il primo) legge dalla pipe i-1 e scrive sulla pipe i. N.B. Nel caso di schema a pipeline e' molto importante che read e write abbiano SUCCESSO! */
    int i, j;                       /* contatori (i NOME STABILITO DAL TESTO!) */
    int fd;                         /* file descriptor */
    int pidFiglio, status, ritorno; /* per valore di ritorno figli */
    char ch;                        /* carattere da cercare e carattere letto da linea (ch NOME STABILITO DAL TESTO!) */
    struttura cur;                  /* array di strutture usate dal figlio corrente (NOME STABILITO DAL TESTO!) */
    int nr, nw;                     /* variabile per salvare valori di ritorno di read su pipe */

    int tmp_conteggio_uno, tmp_conteggio_due;

    /* controllo sul numero di parametri almeno 2 file e un carattere */
    if (argc < 3 || argc % 2 != 0)
    {
        printf("Errore numero di parametri: infatti argc e' %d, deve essere un numero intero positivo pari\n", argc);
        exit(1);
    }

    N = argc - 1; // numero di processi da creare

    /* stampa di debugging */
    printf("DEBUG-Numero di processi da creare %d\n", N);

    /* allocazione pipe */
    if ((pipes = (pipe_t *)malloc(N * sizeof(pipe_t))) == NULL)
    {
        printf("Errore allocazione pipe\n");
        exit(4);
    }

    /* creazione pipe */
    for (i = 0; i < N; i++)
        if (pipe(pipes[i]) < 0)
        {
            printf("Errore creazione pipe\n");
            exit(5);
        }

    /* creazione figli */
    for (i = 0; i < N; i++)
    {
        if ((pid = fork()) < 0)
        {
            printf("Errore creazione figli\n");
            exit(6);
        }
        if (pid == 0)
        { /* codice figlio */
            printf("Sono il figlio di indice %d e pid %d associato al file %s\n", i, getpid(), argv[i + 1]);
            /* nel caso di errore in un figlio decidiamo di ritornare un valore via via crescente rispetto al massimo valore di i (cioe' rispetto N-1) */
            /* chiusura pipes inutilizzate */
            for (j = 0; j < N; j++)
            {
                if (j != i)
                    close(pipes[j][1]);
                if ((i == 0) || (j != i - 1))
                    close(pipes[j][0]);
            }

            /* apertura file */
            if ((fd = open(argv[i + 1], O_RDONLY)) < 0)
            {
                printf("Impossibile aprire il file %s\n", argv[i + 1]);
                exit(N + 1);
            }

            j = 0;

            tmp_conteggio_due = 0,
            tmp_conteggio_uno = 0;

            // adesso, divido in processi dispari e pari
            if (i % 2 == 0 || i == 0)
            {
                // processo pari, deve contare quante volte ci sono caratteri ASCI pari
                // printf("%d tmp_primo conteggio\n", tmp_conteggio_uno);
                // printf("%d tmp_secodno conteggio\n", tmp_conteggio_due);

                // leggo il file
                while (read(fd, &ch, 1) > 0)
                {
                    // printf("sto leggendo il carattere ch: %c, dal file, ed il codice ascii è: %d\n", ch, ch);

                    // printf("j=%d\n", j);

                    if (j % 2 == 0)
                    {
                        if (ch % 2 == 0)
                        {
                            // ho trovato un carattere pari aggiorno la struttura dati
                            tmp_conteggio_uno++;
                            j++;
                            // printf("%d tmp_primo conteggio, ch pari\n", tmp_conteggio_uno);
                        }
                        else
                        {
                            tmp_conteggio_due++;
                            j++;
                            // printf("%d tmp_secodno conteggio, ch dispari\n", tmp_conteggio_due);
                        }
                    }
                    else
                    {
                        j++;
                    }
                }
            }
            else
            {

                // printf("%d tmp_primo conteggio\n", tmp_conteggio_uno);
                // printf("%d tmp_secodno conteggio\n", tmp_conteggio_due);

                // processo dispari, deve contare quante volte ci sono caratteri ASCI dispari

                // leggo il file
                while (read(fd, &ch, 1) > 0)
                {

                    // printf("sto leggendo il carattere ch: %c, dal file, ed il codice ascii è: %d\n", ch, ch);
                    if (j % 2 != 0)
                    {
                        if (ch % 2 != 0)
                        {
                            // ho trovato un carattere pari aggiorno la struttura dati
                            tmp_conteggio_uno++;

                            j++;
                            // printf("%d tmp_primo conteggio, ch pari\n", tmp_conteggio_uno);
                            // rintf("%d tmp_secodno conteggio ch pari\n", tmp_conteggio_due);
                        }
                        else
                        {
                            tmp_conteggio_due++;
                            j++;
                            // printf("%d tmp_primo conteggio, ch dispari\n", tmp_conteggio_uno);
                            // printf("%d tmp_secodno conteggio, ch dispari\n", tmp_conteggio_due);
                        }
                    }
                    else
                    {
                        j++;
                    }
                }
            }

            // printf("sono il figlio di pid %d e ho messo in conteggio uno: %d e conteggio due: %d\n", getpid(), tmp_conteggio_uno, tmp_conteggio_due);

            // controllo che la write non ha avuto problemi

            cur.primo_conteggio = tmp_conteggio_uno;
            cur.secondo_conteggio = tmp_conteggio_due;

            printf("in cur.primo_conteggio c'è: %d\n", cur.primo_conteggio);
            printf("in cur.secondo_conteggio: %d\n", cur.secondo_conteggio);

            // ogni processo  scrive al padre
            nw = write(pipes[i][1], &cur, sizeof(cur));

            if (cur.primo_conteggio > cur.secondo_conteggio)
            {
                exit(0);
            }
            else
            {
                exit(1);
            }

            exit(i); /* ogni figlio deve ritornare al padre il proprio indice */
        }
    } /* fine for */

    /* codice del padre */
    /* chiude lati che non usa */
    for (i = 0; i < N; i++)
        close(pipes[i][1]);

    // il padre deve leggere ogni struttura inviata dal figlio i esimo

    /* si legge prima da pipe pari poi da dispari, come specificato dal testo */
    for (i = 0; i < N; i += 2)
    {
        read(pipes[i][0], &cur, sizeof(cur));
        printf("Figlio con indice pari %d ha calcolato: c1=%ld e c2=%ld\n", i, cur.primo_conteggio, cur.secondo_conteggio);
    }
    for (i = 1; i < N; i += 2)
    {
        read(pipes[i][0], &cur, sizeof(cur));
        printf("Figlio con indice dispari %d ha calcolato: c1=%ld e c2=%ld\n", i, cur.primo_conteggio, cur.secondo_conteggio);
    }

    /* Il padre aspetta i figli */
    for (i = 0; i < N; i++)
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
        {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (se > di %d problemi)\n", pidFiglio, ritorno, N - 1);
        }
    }
    exit(0);
}
