#! user/bin/env sh

echo "Inserisci il nome del file o direttorio da cercare: (con 'fine' esci dal programma)" 
read str

while test $str != "fine"
do
    if test -f $str
    then 
        echo "$str è un file"
    else
        if test -d $str 
        then
            echo "$str è un direttorio"
        else
            echo "$str non esiste"
        fi
    fi

    read str
done