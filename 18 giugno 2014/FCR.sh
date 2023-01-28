#!/bin/sh
#FCR.sh 
#file comandi ricorsivo 

cd $1
#la variabile NR ci serve per il numero di linee
NR=
#la variabile files ci serve per contenere i file trovati
files=
	
for i in *
do
	#controlliamo solo i nomi dei file 
	if test -f $i 
	then 	
		NR=`wc -l < $i`
		if test $NR -eq $3
		#i file devono avere un numero di linee pari a X (terzo parametro)
		then
			#dobbiamo fare l'altro controllo sul nome del file rispetto a S (secondo parametro)
			case $i in
			$2*) ;; #NON va bene che sia all'inizio
			*$2) ;; #NON va bene che sia alla fine
			*$2*)  #va bene che sia solo in mezzo 
				# accumulo i nomi in una variabile
			        files="$files $i";;
			*) ;; #NON va bene che non ci sia 
			esac
		fi
	fi
done
#verifico che la variabile files non sia nulla
if test "$files"
then
#si deve riportare il nome assoluto di questo direttorio sullo standard output e quindi i nomi di tutti i file trovati  
	echo Trovato il direttorio `pwd` che contiene i seguenti file
	echo $files
	#adesso bisogna chiedere conferma all'utente, se bisogna invocare la parte in C
	echo -n "Devo invocare la parte in C? (si'/Si'/yes/Yes) "
	read risposta
	case $risposta in
	s* | S* | y* | Y*) #si deve chiamare la parte C passando come parametri i nomi dei file trovati e la loro lunghezza in linee X  
			echo	18Giu14 $files $3 
				18Giu14 $files $3 ;;
	*) echo nessuna invocazione parte C;;
	esac
fi

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		$0 `pwd`/$i $2 $3
	fi
done

