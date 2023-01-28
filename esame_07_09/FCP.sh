#!/bin/sh
#una possibile soluzione della prova shell del 07 settembre 2022
#attenzione il file deve essere un file con il diritto di esecuzione almeno per l'utente settato

#controllo sul numero di parametri
case $#
in
    0|1|2) echo errore numero di parametri $#, pochi parametri
    exit 1;;
    *) echo ok, proseguiamo con $# parametri
    ;;
esac

#controllo che il primo sia un numero intero positivo dispari
case $1
in
    *[!0-9]*) echo errore carattere non numerico o non postivo
    exit 2;;
    *)  if test $1 -eq 0 -o `expr $1 % 2` -eq 0
        then
            echo errore numero: $1, uguale a zero o numero pari
            exit 3
    fi;;
esac

#nome stabilito dal testo
M=$1
#adesso facciamo lo shift in modo che ci rimangono solo le Gi gerarchie da controllare
shift 

#controllo sulle gerarchie, facendo for G lo script agisce su $@ cioè su tutti gli argomenti della linea di comando
for abs  #abs nome stabilito dal testo
        do
                case $abs in
                /*) if test ! -d $abs -o ! -x $abs
                then
                        echo $abs non direttorio o non accessibile
                        exit 4;
                fi;;
                *) echo argomento sbagliato: $abs non assoluto
                exit 4;;
                esac
done


#controllo sul numero di param finito adesso possiamo passare alle H fasi
PATH=`pwd`:$PATH
export PATH

for abs
do
    echo fase per $abs
    FCR.sh $abs $M
done



