#!/bin/sh
#Soluzione della Prova del 19 giugno 2013

#almeno 3 parametri
case $# in
0 | 1 | 2)
    echo Errore: numero parametri $# quindi pochi parametri.
    exit 1
    ;;
*) echo OK: da qui echo poi proseguiamo con $# parametri ;;
esac

#il primo deve essere un nome assoluto di dir
case $1 in
        /*) if test ! -d $1 -o ! -x $1; then
            echo $1 non directory o non attraversabile
            exit 2
        fi ;;
        *)
            echo $1 non nome assoluto
            exit 3
            ;;
esac


#il secondo deve eesere un numero strettamente positivo
case $2 in
        *[!0-9]*)
            echo $2 non numerico o non positivo
            exit 4
            ;;
        *) if test $2 -eq 0; then
            echo ERRORE: parametro $2 uguale a zero
            exit 5
        fi ;;
esac

if test $2 -lt 2
then
    echo attenzione il numero $2 non va bene, deve essere maggiore o uguale a 2
    exit 6
fi

#il terzo deve eesere un numero strettamente positivo
case $3 in
        *[!0-9]*)
            echo $3 non numerico o non positivo
            exit 4
            ;;
        *) if test $3 -eq 0; then
            echo ERRORE: parametro $3 uguale a zero
            exit 5
        fi ;;
esac

#controlli sui parametri finiti possiamo passare alle Q fasi, dopo aver settato il path
PATH=$(pwd):$PATH
export PATH


FCR.sh $* #invochiamo il file comandi ricorsivo con la gerarchia e i due numeri passati come ultimi due parametri

