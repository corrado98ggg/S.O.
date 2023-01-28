#!/bin/sh
#file principale della prova del 10 Settembre 2014
#controllo numero di parametri
case $# in
   3);; #numero di parametri corretto
   *) echo "ERRORE: numero di parametri errato"
      exit 1;;
esac

#controllo primo parametro
case $1 in
   /*) #Ã¨ un nome assoluto, controllo che sia un direttorio esplorabile
       if test ! -d $1 -o ! -x $1
       then
          echo "ERRORE: $1 direttorio non esistente o non esplorabile"
          exit 2
       fi;;
   *) #non Ã¨ un nome assoluto
      echo "ERRORE: $1 non Ã¨ un nome assoluto"
      exit 3;;
esac

#controllo il secondo parametro nello stesso modo in cui ho controllato il primo
case $2 in
   /*) if test ! -d $2 -o ! -x $2
       then
          echo "ERRORE: $2 direttorio non esistente o non esplorabile"
          exit 4
       fi;;
   *) echo "ERRORE: $2 non Ã¨ un nome assoluto"
      exit 5;;
esac

#controllo terzo parametro
expr $3 + 0 >/dev/null 2>&1
tmp=$?
if test $tmp -ne 2 -a $tmp -ne 3
then
   #Ã¨ numerico, controllo sia strettamente positivo
   if test $3 -le 0
   then
      #non Ã¨ positivo
      echo "ERRORE: $3 non strettamente positivo"
      exit 6
   fi
else
   echo "ERRORE: $3 non numerico"
   exit 7
fi
#fine controlli sui parametri

export PATH=`pwd`:$PATH
#creo/azzero un file temporaneo
> /tmp/filetmp$$

#invoco due volte il file comandi ricorsivo per esplorare separatamente G1 e G2
#ad entrambi passo il file temporaneo come terzo parametro
FCR.sh $1 $3 /tmp/filetmp$$
FCR.sh $2 $3 /tmp/filetmp$$

#invoco la parte c
#il primo parametro Ã¨ l'elenco dei files trovati, il secondo parametro Ã¨ il valore H
10Set14 `cat /tmp/filetmp$$` $3

#rimuovo il file temporaneo visto che non serve piÃ¹
rm /tmp/filetmp$$