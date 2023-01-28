#CONTROLLO NUMERO DI PARAMETRI
#MODO 1:

case $# in #esempio 4 parametri
4) ;;
*) echo Errore: ci vogliono 4 argomenti >&2
 exit 1;;
esac

case $# in #esempio 4 parametri
0|1|2|3) echo Errore: pochi argomenti >&2
 exit 1;;
4) ;;
*) echo Errore: troppi argomenti >&2
 exit 1;;
esac


#CONTROLLO NOME ASSOLUTO E DIRETTORIO

case $1 in #esempio primo parametro
/*) if test ! -d $1 -o ! -x $1
 then
 echo $1 non direttorio o non accessibile >&2
 exit 2;
 fi;;
*)
echo argomento sbagliato: $1 NON assoluto >&2
 exit 3;;
esac

#se ho da controllare più di una gerarchia e in particolare
#tutti i parametri che ci sono sono nomi assoluti di dir
for G in $*
do
    case $G in #esempio primo parametro
    /*) if test ! -d $G -o ! -x $G
    then
    echo $G non direttorio o non accessibile >&2
    exit 2;
    fi;;
    *)
    echo argomento sbagliato: $G NON assoluto >&2
    exit 3;;
    esac
done


#CONTROLLO NOME ASSOLUTO E FILE

case $2 in #esempio secondo parametro
/*) if test ! -f $2
 then echo $2 non file >&2
 exit 4
 fi;;
*) echo argomento sbagliato: $1 NON assoluto >&2
 exit 5;;
esac


#CONTROLLO NOME RELATIVO SEMPLICE

case $3 in #esempio terzo parametro
*/*) echo argomento sbagliato: $3 nome NON relativo semplice >&2
 exit 6;;
*) ;;
esac

#CONTROLLO NUMERO

# ogni carattere di una stringa che deve rappresentare
# un valore numerico deve essere numerico
# un solo carattere non numerico risulta in un errore

case $4 in
*[!0-9]*)
echo Errore in argomento numerico: $4 >&2
exit 7;;
*) #controllo se il numero è zero
    if test $4 -eq 0
    then
        echo errore: parametro uguale a zero
        exit 8
    fi;;
esac


#CONTROLLO CARATTERE

case $5 in #esempio quinto parametro
?) ;;
*) echo Argomento sbagliato: $5 NON un carattere >&2
 exit 8;;
esac


#CONTROLLO STRINGA NON NULLA 

if test -z "$a" #esempio variabile a
 then
 echo La variabile a risulta nulla >&2
fi

#CONTROLLO FILE LEGGIBILE CHE CONTENGA UN OCCORRENZA DI C

for f in *
do
    if test -f $f -a -r $f
    then
        if grep $2 $f >/dev/null 2>&1
        then
            echo'pwd'/$f >> $3 #salviamo il nome assoluto nel file tmp
            contafile='expr $contafile +1' #sia qui sia a quello di sopra vanno gli apici traversi!!!
        fi
    fi
done

#CONTROLLO ALFABETICO MINUSCOLO

case $1 in
    ?)  echo $1 singolo carattere, ora controlliamo alfabetico minuscolo
        case $1 in
            [!a-z]) echo $1 non alfabetico minuscolo
            exit 2;;
            *) echo $1 carattere alfabetico minuscolo
            exit 3;;
        esac;;
    *)  echo $1 sbagliato, non singolo carattere
    exit 4;;
esac