#!/bin/sh
#Soluzione dell'esame del 16 giugno 2021

case $# in
0 | 1 | 2 | 3)
    echo Errore: numero parametri is $# quindi pochi parametri.
    exit 1
    ;;
*) echo DEBUG-da qui, proseguiamo con $# parametri ;;
esac

#Controllo primo parametro sia un numero strettamente positivo
case $1 in
*[!0-9]*)
    echo $1 non numerico o non positivo
    exit 2
    ;;
*) if test $1 -eq 0; then
    echo errore: numero uguale a zero
    exit 3
fi ;;
esac

H=$1 #salviamo il primo parametro (N.B. nella variabile il cui nome viene specificato nel testo)

#Controllo secondo parametro sia un numero strettamente positivo
case $2 in
*[!0-9]*)
    echo $2 non numerico o non positivo
    exit 4
    ;;
*) if test $2 -eq 0; then
    echo errore $2: numero uguale a zero
    exit 5
fi ;;
esac


M=$2 #salviamo il numero nella variabile indicata dal testo

#quindi ora possiamo usare il comando shift due volte
shift
shift

#ora in $* abbiamo solo i nomi assoluti di dir e quindi possiamo fare i controlli
for G; do
    case $G in
    /*) if test ! -d $G -o ! -x $G; then
        echo $G non direttorio o non accessibile
        exit 3
    fi ;;
    *)
        echo argomento sbagliato: $G non assoluto
        exit 4
        ;;
    esac
done

#controlli sui parametri finiti
PATH=$(pwd):$PATH
export PATH

#invochiamo il file comandi ricorsivo con la gerarchia, il numero, il file temporaneo e i caratteri
#NOTA BENE: i caratteri li dobbiamo passare per ultimi!!!
for G; do
    echo fase per $G
    #invochiamo il file comandi ricorsivo passando i parametri
    FCR.sh $G $H $M
done
