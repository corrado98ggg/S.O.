#!/bin/sh
#file comandi ricorsivo per la prova del 10 Settembre 2014 
cd $1

numlinee=0 # serve per contare il numero di linee di ogni file
trovato=0 #per sapere se il direttorio corrente corrisponde alle specifiche

for i in *
do
   #controllo che sia un file 
   if test -f $i 
   then
      #controllo il numero di linee
      numlinee=`wc -l < $i`
      if test $numlinee -ge $2
      then
         #aggiungo il nome assoluto del file trovato al file temporaneo e aggiorno la variabile $trovato
         trovato=1
         echo `pwd`/$i >> $3
      fi
   fi
done

#controllo la variabile $trovato
if test $trovato -eq 1
then
   echo "Trovato trovato: `pwd`"
fi

for i in *
do
   if test -d $i -a -x $i
   then
      #invocazione ricorsiva
      $0 `pwd`/$i $2 $3
   fi
done