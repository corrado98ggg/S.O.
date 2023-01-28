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



#prima variabile deve essere un nome assoluto di gerarchia

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

#adesso dobbiamo controllare che ci siano $* nomi relativi semplici
for i in $*
do
    case $i in
    */*) echo argomento sbagliato $i non relativo semplice >&2
    exit 4;;
    *) if ! test $i -f
    then
        echo attenzione non è un file
        exit 5;
    fi
    files="$files $i";;
    esac
done


#controlli sui parametri finiti possiamo passare alle Q fasi, dopo aver settato il path
PATH=$(pwd):$PATH
export PATH

echo fase per $G
    FCR.sh $G $files