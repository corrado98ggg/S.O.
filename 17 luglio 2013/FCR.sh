#!/bin/sh
#FCR.sh
#file comandi ricorsivo

cd $1
cont=0
filess= #i files che troviamo uguali ce li salviamo per poter passarli alla parte C

#in $2 c'è una variabile con tutti i nomi dei file
G=$1

shift

for i in $G
do
    for f in $*
    do

        numero_file=$(expr $numero_file +1)
        if test $f -eq $i
        then
            echo $f e $i sono uguali aumentiamo il contatore
            cont=$(expr $cont +1)
            filess="$filess $f"
        fi
    done
done

numero_file=$(expr $numero_file / 2)

if test $cont -ge numero_file
then

    echo bene ci sono almeno la metà dei file, possiamo chiamare la parte c
    main $filess

fi

#se DIR e a true e files non e' vuoto allora abbiamo trovato una directory giusta
if test $DIR = true -a $trovato = true; then
    echo TROVATO dir $(pwd)
    for F in $files; do
        #creaiamo il file con il nome richiesto
        >$F.Chiara
        L=$(wc -c <$F)
       # echo chiamiamo la parte C con file $F, L uguale a $L e $B
        #17Giu20 $F $L $B
    done
fi

for i in *; do
    if test -d $i -a -x $i; then
        #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto della directory
        FCR.sh $(pwd)/$i $2
    fi
done
