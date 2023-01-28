#!/bin/sh
#Soluzione dell'esame del 16 febbraio 2022

case $# in
0 | 1 | 2)
    echo Errore: numero parametri is $# quindi pochi parametri.
    exit 1
    ;;
*) echo DEBUG-da qui, proseguiamo con $# parametri ;;
esac

#Controllo primo parametro sia un carattere MINuscolo
case $1 in
?)
    echo $1 singolo carattere, adesso controllo se minuscolo:
    case $1 in
    [!a-z])
        echo $1 non alfabetico minuscolo
        exit 2
        ;;
    *)
        echo ok, alfabetico minuscolo
        ;;
    esac
    ;;

*)
    echo $1 sbagliato, non singolo carattere
    exit 3
    ;;
esac

C=$1 #salviamo il primo parametro (N.B. nella variabile il cui nome viene specificato nel testo)
shift

#ora in $* abbiamo solo i nomi assoluti di dir e quindi possiamo fare i controlli
for G; do
    case $G in
    /*) if test ! -d $G -o ! -x $G; then
        echo $G non direttorio o non accessibile
        exit 8
    fi ;;
    *)
        echo argomento sbagliato: $G non assoluto
        exit 9
        ;;
    esac
done

#controlli sui parametri finiti
PATH=$(pwd):$PATH
export PATH

#azzeriamo il contenuto del file tmp
>/tmp/nomiAssoluti

#invochiamo il file comandi ricorsivo con la gerarchia, il numero, il file temporaneo e i caratteri
#NOTA BENE: i caratteri li dobbiamo passare per ultimi!!!
for G; do
    echo fase per $G
    #invochiamo il file comandi ricorsivo passando i parametri
    FCR.sh $G $C /tmp/nomiAssoluti
done

N=`wc -l < /tmp/nomiAssoluti`


#contiamo il numero di file trovati
echo il numero di file trovati sono $N


if test $N -ge 2; then
    #invoco la parte C passando, i nomi assoluti di tutti i file, e il carattere
    echo sto per invocare la parte C
    main $(cat /tmp/nomiAssoluti) $C
fi

#cancelliamo il file temporaneo
rm /tmp/nomiAssoluti
