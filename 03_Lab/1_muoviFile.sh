#!bin/bash sh

#controllo input
if (test $# -lt 5)
    then 
    echo "Argomenti insufficienti: sh muoviFile num dirSorgente dirDestinazione ext1 ext2 extN"
    exit 1
fi

if (test $1 -lt 0)
    then
    echo "Argomento <num> deve essere un numero intero"
    exit 2
fi

if (test ! -d $2)
    then
    echo "Argomento directory sorgente inesistente"
    exit 3
fi

if (test ! -d $3)
    then
    echo "Argomento directory destinazione inesistente"
    exit 4 
fi

#Inizializzo variabili
num=$1
dir_sor=$2
dir_dest=$3
shift 3
list_ext=$*

#Aggiunta della directory corrente alla variabile PATH
currentPath="$(pwd)/4_esame_rec.sh"
PATH=$PATH:$currentPath
export PATH

#Creazione del file di appogio temporaneo
touch "$dir_sor/counter.tmp"

#Chiamata a ricerca.sh
sh "./1_ricerca.sh" "$num" "$dir_sor" "$dir_dest" $list_ext

#Stampa del numero di righe del file counter.tmp e rimozione del file counter.tmp
echo "$(wc -l "$dir_sor/counter.tmp")"
rm "$dir_sor/counter.tmp"