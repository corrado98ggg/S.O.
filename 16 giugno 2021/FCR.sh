#!/bin/sh
#FCR.sh
#file comandi ricorsivo

cd $1

files= #ci serve per tenere conto dei nomi dei files
cont=0 #nome scelto dal testo per il numero di file trovati

for F in *; do

    if test -f $F; then
        L=$(wc -c <$F)          #numero in caratteri del file
        if test $L -eq $2; then #dobbiamo vedere se il nome è costituto da due caratteri
            case $F in
            ??) #allora abbiamo trovato il file
                $cont=$(expr $cont + 1)
                #stampiamo il nome del file su standard output
                echo abbiamo trovato un il file $(pwd)/$F che soddisfa le caratteristiche
                files="$files $i"
                ;;
            *) ;; 
            esac
        fi
    fi
done

#chiamiamo il file C, se e solo se cont è maggiore o uguale di 2 ed minore di M
if test $cont -lt $3 -a $cont -ge 2; then

    #chiamiamo il file c passando le variabili:
    echo inchiamo il main $files

    main $files

fi

for i in *; do
    if test -d $i -a -x $i; then
        #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio corrente
        $0 $(pwd)/$i $2 $3 #ATTENZIONE AI PARAMETRI!
    fi
done
