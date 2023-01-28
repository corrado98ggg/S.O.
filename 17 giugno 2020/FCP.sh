#!/bin/sh
#Soluzione della Prova del 17 Luglio 2020

case $# in
0 | 1 | 2)
    echo Errore: numero parametri $# quindi pochi parametri. Usage is $0 H K dirass1 dirass2 ...
    exit 1
    ;;
*) echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#dobbiamo isolare l'ultimo parametro e intanto facciamo i controlli

num=1 #la variabile num ci serve per capire quando abbiamo trovato l'ultimo parametro

params= #la variabile params ci serve per accumulare i parametri a parte l'ultimo

#in $* abbiamo i nomi delle gerarchie e i due numeri interi
for i; do
    if test $num -ne $#; then #ci serve per non considerare gli ultimi due parametri che sono i numeri
        #soliti controlli su nome assoluto e directory traversabile
        case $i in
        /*) if test ! -d $i -o ! -x $i; then
            echo $i non directory o non attraversabile
            exit 2
        fi ;;
        *)
            echo $i non nome assoluto
            exit 3
            ;;
        esac
        params="$params $i" #se i controlli sono andati bene memorizziamo il nome nella lista params
    else
        #abbiamo individuato l 'ultimo parametro e quindi facciamo il solito controllo su numerico e strettamente positivo
        case $i in
        *[!0-9]*)
            echo $i non numerico o non positivo
            exit 4
            ;;
        *) if test $i -eq 0; then
            echo ERRORE: parametro $i uguale a zero
            exit 5
        fi ;;
        esac
        #se i controlli sono andati bene salviamo ultimo parametro
        if test $num -eq $(expr $#); then
            B=$i

        fi
    fi
    num=$(expr $num + 1) #incrementiamo il contatore del ciclo sui parametri
done

#controlli sui parametri finiti possiamo passare alle Q fasi, dopo aver settato il path
PATH=$(pwd):$PATH
export PATH

#ora in $params abbiamo solo i nomi delle gerarchie
for G in $params; do
    echo fase per $G
    FCR.sh $G $B #invochiamo il file comandi ricorsivo con la gerarchia e i due numeri passati come ultimi due parametri
done
