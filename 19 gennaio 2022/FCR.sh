#!/bin/sh
#FCR.sh
#file comandi ricorsivo del 19 gennaio 2022

cd $1

files= #ci serve per tenere conto dei nomi dei files
cont=0 #nome scelto dal testo per il numero di file trovati

for F in *; do

    if test -f $F -a -r $F; then
        L=$(wc -c <$F) #numero in caratteri del file
        if test $L -eq $3 -a $(expr $L % $2) -eq 0; then
            #abbiamo trovato un file che soddisfa i requisiti
            $cont= $(expr $cont +1)

            #scrivo il nome assoluto del file nel tmp
            echo salviamo il file nel tmp `pwd`/$F >> $4
        fi
    fi
done


if test $cont -ge 1
then
echo trovata directory con nome assoluto `pwd`
fi

for i in *; do
    if test -d $i -a -x $i; then
        #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio corrente
        $0 $(pwd)/$i $2 $3 $4 #ATTENZIONE AI PARAMETRI!
    fi
done
