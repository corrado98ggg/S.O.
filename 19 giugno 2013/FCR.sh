#!/bin/sh
#file comandi ricorsivo 

cd $1
#la variabile l conta la lunghezza in byte dei file
l=
#la variabile trovato conta i file con le caratteristiche richieste
trovato=0
#la variabile files contiene i nomi dei file con le caratteristiche richieste
files=
#la variabile ok se a 1 indica che il file rispetta le specifiche, altrimenti varra' 0
ok=

for i in *
do
	#la variabile ok se rimane a 1 vuole dire che il file ha le caratteristiche specificate
	ok=1
	#controlliamo solo i file
	if test -f $i 
	then 	
		#calcoliamo la lunghezza in byte
		l=`wc -c < $i`
		if test $l -eq $3
		then
			#verifichiamo che il contenuto del file (`cat $i`) sia costituito solo di caratteri numerici
			case `cat $i` in
			*[!0-9]*) ok=0;; #caratteri non corretti e quindi cambiamo valore alla variabile OK
			*) ;; #caratteri corretti e non facciamo nulla
			esac
	
			if test $ok -eq 1
			then
				trovato=`expr $trovato + 1`
				files="$files $i"
			fi
		fi
	fi
done
if test $trovato -ge $2
then
	#abbiamo trovato un direttorio che soddisfa le specifiche e quindi stampiamo il suo nome assoluto e chiamiamo la parte C
	echo Trovato direttorio `pwd`
	echo Stiamo per invocare la parte C con i seguenti file $files
	19Giu13 $files
fi

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		19giu13r.sh `pwd`/$i $2 $3
	fi
done