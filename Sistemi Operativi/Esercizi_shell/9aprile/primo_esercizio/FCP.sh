#!/bin/sh
#creazione del file FCP.sh per il controllo e la stampa dei parametri
#numero di parametri 2:
#primo nome assoluto di una dir, che identifica una gerarchia G
#il secondo un numero intero X strettamente positivo.

#primo check, numero di parametri 2:

case $# in
2) ;;
*) 	echo errore: ci vogliono almeno due parametri >&2
	exit 1;;
esac

#secondo check, controlliamo che il primo parametro contenga il nome assoluto di una dir

case $1 in
/*) if test ! -d $1 -o ! -x $1
	then echo $1 non direttorio o non accesssibile >&2
	exit 2	
    fi;;
*)
	echo argoemnto sbagliato: $1 NON assoluto >&2
	exit3 3;;
esac 



#adesso controlliamo che il secondo parametro sia un numero X:

#il comando expr puo' verificare una espressione numerica
#expr restituisce lo stato 0 in caso di successo
#valore 1 per risutlato 0; 2 in caso di indsuccesso (per esempio opernado alfanumerico)

expr $2 + 0 >/dev/null 2>&1
if test $? -eq 2
then 
	echo errore in argomento numerico: $2 >&2
	exit 7
fi

#adesso dobbiamo controllare che sia maggiore di zero:

if test $2 -lt 0
then
	echo Attenzione il numero è negativo
	exit 8
fi

#modifichiamo ed esportiamo la variabile PATH
PATH=`pwd`:$PATH
export PATH

#anche se non richiesto specificamente per completezza stampiamo il numero e il nome dei file trovati
echo abbiamo trovato `wc -l < /tmp/tmp$$` file che soddisfano le specifiche

#chiamo il file ricorsivo:
FCR.sh $*
