#!/bin/sh
#Soluzione dell'esame del 8 settembre 2021

#controllo sul numero di parametri
case $# in
0 | 1 | 2)
    echo Errore: numero parametri is $# quindi pochi parametri.
    exit 1
    ;;
*) echo DEBUG-da qui, proseguiamo con $# parametri ;;
esac

#Controllo primo parametro sia un singolo carettere
case $1 in
?) ;;
*)
    echo errore: il parametro passato non un singolo carattere
    ;;
esac

C=$1 #salviamo il primo parametro (N.B. nella variabile il cui nome viene specificato nel testo)

#effettuiamo lo shift
shift

#adesso ci sarano solo i nomi assoluti da controllare
for G in $*; do

    case $G in

    /*) if test ! --d $G -o ! -x $G; then
        echo argomento $G non direttorio o non accessibile
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


#azzerriamo il file temporaneo
>/tmp/nomiAssoluti

#invochiamo il file comandi ricorsivo con la gerarchia, il numero, il file temporaneo e i caratteri
for G in $*; do
    echo fase per $G
    #invochiamo il file comandi ricorsivo passando i parametri
    FCR.sh $G $C /tmp/nomiAssoluti
done


#N.B. Andiamo a contare le linee del file temporaneo
#dato che il numero di directory trovate lo dobbiamo stampare e poi lo dobbiamo controllare lo salviamo in una variabile
echo il numero di directory trovate nelle gerarchie = `wc -l < /tmp/nomiAssoluti`

#ora per ogni directory trovata dobbiamo invocare la parte C passando solo i file contentuti
files= #variabile di appoggio
for i in `cat /tmp/nomiAssoluti`
do

    cd $i #ci spostiamo nella directory corrente
    files=
    for j in *
    do


        if test -f $j #se il file è leggibile
        then
            files="$files $i"
        fi
    done

    echo adesso invochiamo la parte C con $files
    main $files
done



#cancelliamo il nome del file assoluto
rm /tmp/nomiAssoluti
