#!/bin/sh
#file comandi ricorsivo della prova del 07 settembre

cd $1

trovato=0 #varabile per il conteggio dei file trovati
posizione=1 #definisco questa variabile per tenere traccia dei file
files= #nome all'interno del quale salviamo i nomi dei file che soddisfano le richieste

for Item in *
do
    if test -f $Item -a -r $Item
    then
        #calcoliamo il numero di caratteri
        L=`wc -c < $Item`
        if test `expr $posizione % 2` -ne 0
            then
                if test $L -eq $2
                then
                    #aggiorniamo il contatore
                    files="$files $Item"
                    #stampiamo
                    echo trovato direttorio `pwd`/$Item 
                    trovato=`expr $trovato + 1`
                fi
        fi
    #aggiorniamo la varabile che tiene conto della posizione
    posizione=`expr $posizione + 1`
    fi
done


if test $trovato -ge 1
then

   echo debug: TROVATO direttorio `pwd`
    #dobbiamo invocare la parte C
   echo invochiamo main $files 
   main $files
fi

for i in *
do
    if test -d $i -a -x $i
    then
        echo stiamo per esplorare la directory `pwd`/$i
        FCR.sh `pwd`/$i $2
    fi
done
