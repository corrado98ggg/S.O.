#!/bin/sh
# Soluzione della prova d'esame del 14 settembre 2011 
#
# File comandi.
#

# controllo sul numero dei parametri
case $# in
0|1|2)  echo "Errore nel numero dei parametri $#"
      	exit 1;;
*)  	echo "Numero parametri corretti $#";;
esac

#CONTROLLO ALFABETICO MINUSCOLO

case $1 in
    ?)  echo $1 singolo carattere, ora controlliamo alfabetico minuscolo
        case $1 in
            [!a-z]) echo $1 non alfabetico minuscolo
            exit 2;;
            *) echo $1 carattere alfabetico minuscolo
            exit 3;;
        esac;;
    *)  echo $1 sbagliato, non singolo carattere
    exit 4;;
esac

C=$1

#ora possiamo usare il comando shift
shift

#se ho da controllare più di una gerarchia e in particolare
#tutti i parametri che ci sono sono nomi assoluti di dir
for G in $*
do
    case $G in #esempio primo parametro
    /*) if test ! -d $G -o ! -x $G
    then
    echo $G non direttorio o non accessibile >&2
    exit 2;
    fi;;
    *)
    echo argomento sbagliato: $G NON assoluto >&2
    exit 3;;
    esac
done


#controlli sui parametri finiti possiamo passare alle N fasi
# imposto la path
PATH=`pwd`:$PATH
export PATH

#azzerriamo il contenuto del file temporaneo
>/tmp/nomiAssoluti

for $G in $*
do
    echo invocazione il file comandi ricorsivo con la gerarchia, il numero e il file temporaneo
    FCR.sh $G $C 
done


echo il numero di file trovati e $(wc -l < /tmp/nomiAssoluti)

if $(wc -l < /tmp/nomiAssoluti) -ge 2
then
    main $(cat /tmp/nomiAssoluti) $C
fi

#rimuovo il file temp
rm /tmp/nomiAssoluti