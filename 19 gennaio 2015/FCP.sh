#!/bin/sh
#Soluzione della Prova del 19 GENNAIO 2015

case $# in
0 | 1 | 2| 3)
    echo Errore: numero parametri $# quindi pochi parametri.
    exit 1
    ;;
*) echo OK: da qui in poi proseguiamo con $# parametri ;;
esac


#primo e secondo nome assoluti di dir G1 e G2


fileG1=0
fileG2=0
files=

#primo param
case $1 in

/*) if test ! -d $1 -o ! -x $1; then
            echo $1 non directory o non attraversabile
            exit 2
        fi ;;
        *)
            echo $1 non nome assoluto
            exit 3
            ;;
esac


G1=$1 #nome impostato dal testo

shift

#primo param
case $2 in

/*) if test ! -d $2 -o ! -x $2; then
            echo $2 non directory o non attraversabile
            exit 2
        fi ;;
        *)
            echo $2 non nome assoluto
            exit 3
            ;;
esac

G2=$2 #nome impostato dal testo

shift

#terzo e quarto parametro devono essere singoli caratteri
#terzo: C1
case $3 in
?) ;; #singolo carattere
*) echo argomento sbagliato $3 non è un singolo carattere
exit ;;
esac
C1=$3


case $4 in
?) ;; #singolo carattere
*) echo argomento sbagliato $4 non è un singolo carattere
exit ;;
esac
C2=$4




#controlli sui parametri finiti possiamo passare alle Q fasi, dopo aver settato il path
PATH=$(pwd):$PATH
export PATH


#adesso azzeriamo un file temporaneo
>/tmp/nomiAssoluti

#devono esserci due fasi successive:
echo invochiamo il file comandi ricorsivo, per la prima fase
FCR.sh $G1 $C1 /tmp/nomiAssoluti


for F in $(cat /tmp/nomiAssoluti)
do
    fileG1=$(expr $fileG1 +1)
    files="$files $F"
done

#adesso azzeriamo un file temporaneo
>/tmp/nomiAssoluti

#devono esserci due fasi successive:
echo invochiamo il file comandi ricorsivo, per la prima fase
FCR.sh $G1 $C2 /tmp/nomiAssoluti

for F in $(cat /tmp/nomiAssoluti)
do
    fileG2=$(expr $fileG1 +1)
    files="$files $F"

done

if test $fileG1 -eq $fileG2 then

    #solo se il numero di file trovati in g1 e g2 è uguale chiamo il C
    echo invochiamo il main
    main $files $C1 $C2
fi

rm /tmp/nomiAssoluti


