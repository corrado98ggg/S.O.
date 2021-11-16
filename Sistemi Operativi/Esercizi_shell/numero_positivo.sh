#!/bin/sh
#questo è un esercizio che ci controlla che un solo parametro sia un numero
#strettamento positvo:

#controllo che sia un solo parametro passato:
case $# in
1) ;;
*) echo errore: numero di parametri deve essere 1 >&2
exit;;
esac

#adesso vediamo se è un numero:
expr $1 + 0 >/dev/null 2>&1
if test $? -eq 2
then 
	echo errore in aromento numerico: $1 >&2
exit 2
fi

#infine vediamo se è positivo:
if test $1 -lt 0
then 
	echo numero minore di 0.
exit 3
fi

echo numero positivo!!!
exit 4

