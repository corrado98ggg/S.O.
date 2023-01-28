#!/bin/sh
#FCR.sh
#file comandi ricorsivo del 19 gennaio 2022

cd $1

cont=0 #nome scelto dal testo per il numero di file trovati

for F in *; do

    if test -f $F -a -r $F; then

        if grep $2 $F >/dev/null 2>&1; then
            echo $(pwd)/$F >>$3 #stampiamo il nome, e lo salviamo nel file tmp
            cont=$(expr $cont +1)
        fi
    fi
done

if test $cont -ge 1; then
    echo trovata directory con nome assoluto $(pwd)
fi

for i in *; do
    if test -d $i -a -x $i; then
        #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio corrente
        $0 $(pwd)/$i $2 $3 #ATTENZIONE AI PARAMETRI!
    fi
done
