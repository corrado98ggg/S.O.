#!/bin/sh
# Soluzione della prova d'esame del 15 Gennaio 2020
#
# File ricorsivo. Uso: $0 dirass H filetemp
#
cd $1

cont= #lunghezza in caratteri di un file

for i in *
do
   # analizzo i file della directory corrente
   if test -f $i -a -r $i	#nota bene la specifica con richiede che sia un file leggibile ... chiaramente si puo' poi avere un porblema con il comando wc!
   then

            if grep $2 $i >/dev/null 2>&1
            then
                #echo "Trovato file `pwd`/$i" #stampa richiesta dal testo 
                # aggiungiamo il nome assoluto del file nel file temporaneo
                cont=$(expr $cont +1)

                #lo mettiamo nel tmp
                $i >> /tmp/nomiAssoluti

            fi
   fi
done


if test $cont -ge 1 
then
    echo abbiamo trovato almeno un file nella dire $(pwd)
fi

# invocazione ricorsiva nella gerarchia
for i in *
do 
    if test -d $i -a -x $i
    then
        FCR.sh `pwd`/$i $2 $3 
    fi
done
