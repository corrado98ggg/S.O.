#!/bin/sh
#FCR.sh
#file comandi ricorsivo


for i in *; do
    #controlliamo solo i nomi dei file (se inseriamo anche il controllo se leggibili per cautelarci dato che dopo usiamo il comando wc bisogna commentarlo!)
    if test -f $i; then

        #DOBBIAMO VEDERE SE IL FILE CONTIENE NEL SUO CONTENUTO ALMENO UN OCCORRENZA DEL CARATTERE

        if grep $2 $F >/dev/null 2>&1
        then
            echo abbiamo trovato il file : $(pwd)/$F >> $3
            cont=$(expr $cont +1) #aggiorniamo il contatore
            #mettiamo il nome all'interno del file temporaneo

        fi
    fi
done

for i in *; do
    if test -d $i -a -x $i; then
        #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto della directory
        FCR.sh $(pwd)/$i $2 $3
    fi
done
