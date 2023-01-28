#!/bin/sh
#FCR.sh
#file comandi ricorsivo

cd $1

cont=0  #nome scelto dal testo per il numero di file trovati

#per prima cosa vediamo se il nome della gerarchia possiede 3 caratteri
case $1 in
*/$2?$2) #il nome ha tre caratteri, ed il primo e il terzo sono C

    for F in *; do
        if test -f $F -a -r $F; then #se è un file abbiamo trovato un file che soddisfa le specifiche
            $cont=$(expr $cont + 1)
        fi
    done
    ;;
esac


if test $cont -ge 1 #se abbiamo trovato almeno un file 
then 
echo directory trovata con nome assoluto `pwd`
#scriviamo il nome assoluto all'interno del file
pwd >> $3 #non serve scrivere echo `pwd` >> $3 in quanto il comando pwd stampa già di suo
fi



for i in *; do
    if test -d $i -a -x $i; then
        #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio corrente
        $0 $(pwd)/$i $2 $3 #ATTENZIONE AI PARAMETRI!
    fi
done
