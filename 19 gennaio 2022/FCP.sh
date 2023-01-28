#!/bin/sh
#Soluzione dell'esame del 19 gennaio 2022

case $# in
0 | 1 | 2 | 3)
    echo Errore: numero parametri is $# quindi pochi parametri.
    exit 1
    ;;
*) echo DEBUG-da qui, proseguiamo con $# parametri ;;
esac

#Controllo primo parametro sia un numero strettamente positivo dispari
case $1 in
*[!0-9]*)
    echo $2 non numerico o non positivo
    exit 2
    ;;
*) if test $2 -eq 0; then
    echo errore $2: numero uguale a zero
    exit 3
fi ;;
esac

if test $(expr $2 % 2) -eq 0; then
    echo attenzione il numero passato è pari, deve essere dispari
    exit 4
fi


C=$1 #salviamo il primo parametro (N.B. nella variabile il cui nome viene specificato nel testo)

#Controllo secondo parametro sia un numero strettamente positivo pari
case $2 in
*[!0-9]*)
    echo $2 non numerico o non positivo
    exit 5
    ;;
*) if test $2 -eq 0; then
    echo errore $2: numero uguale a zero
    exit 6
fi ;;
esac

if test $(expr $2 % 2) -ne 0; then
    echo attenzione il numero passato è dispari, deve essere pari
    exit 7
fi

X=$2 #salviamo il numero nella variabile indicata dal testo

#quindi ora possiamo usare il comando shift due volte
shift
shift

#ora in $* abbiamo solo i nomi assoluti di dir e quindi possiamo fare i controlli
for G; do
    case $G in
    /*) if test ! -d $G -o ! -x $G; then
        echo $G non direttorio o non accessibile
        exit 8
    fi ;;
    *)
        echo argomento sbagliato: $G non assoluto
        exit 9
        ;;
    esac
done

#controlli sui parametri finiti
PATH=$(pwd):$PATH
export PATH

#azzeriamo il contenuto del file tmp
>/tmp/nomiAssoluti

#invochiamo il file comandi ricorsivo con la gerarchia, il numero, il file temporaneo e i caratteri
#NOTA BENE: i caratteri li dobbiamo passare per ultimi!!!
for G; do
    echo fase per $G
    #invochiamo il file comandi ricorsivo passando i parametri
    FCR.sh $G $C $X /tmp/nomiAssoluti
done

#contiamo il numero di file trovati
echo il numero di file trovati sono $(wc -l < /tmp/nomiAssoluti)

#ora per ogni file trovato dobbiamo invocare la parte C, passando come param i nomi assoluti dei file e il numero C

#io ho fatto così
for i in $(cat /tmp/nomiAssoluti); do
    echo ivochiamo il main passando $i e il parametro $C
    main $i $C
done

#l'esame faceva così
#ora dobbiamo invocare la parte C passando C e i file trovati
echo Sto per invocare 19Gen22 `cat /tmp/nomiAssoluti` $C
19Gen22 `cat /tmp/nomiAssoluti` $C

#cancelliamo il file temporaneo
rm /tmp/nomiAssoluti


