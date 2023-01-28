#!/bin/sh
#Soluzione della Prova del 18 Giugno 2014
#versione che usa un file temporaneo e usa FCR.sh per tutte le fasi

case $# in
3) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
*)	echo Errore: numero parametri is $# quindi pochi/troppi parametri. Usage is $0 dirass stringa numero
	exit 1;;
esac

#Controllo G, cioe' $1
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    echo $1 non direttorio
    exit 2
    fi;;
*)  echo $1 non nome assoluto; exit 3;;
esac

#puo' avere senso controllare che il secondo parametro S non contenga un carattere /
case $2 in
*/*) echo Errore: $f non in forma relativa semplice
    exit 3;;
*) ;;
esac

#Controllo X, cioe' $3
case $3 in
*[!0-9]*) echo non numerico o non positivo
	  exit 4;;
*) ;;
esac
if test $3 -eq 0
then
	echo Numero fornito $3 uguale a zero
	exit 5
fi
if test $3 -ge 255
then
	echo Numero fornito $3 non strettamente minore di 255 
	exit 6
fi

#controlli sui parametri finiti

PATH=`pwd`:$PATH
export PATH

#invochiamo il file comandi ricorsivo con la gerarchia, la stringa e il numero passati come parametri
FCR.sh $*
