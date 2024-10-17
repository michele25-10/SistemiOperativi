#!bin/bash sh

#controllo input
if (test $# -lt 3)
    then
    echo "sh cercaFile <stringa> <dir> <num>"
    exit 1
fi

if (test -z $1)
    then
    echo "<stringa> deve essere una string non vuota"
    exit 2
fi

if (test ! -d $2)
    then
    echo "<dir> deve essere un percorso assoluto esistente"
    exit 2
fi

if (test $3 -lt 0)
    then
    echo "<num> deve essere un numero intero"
    exit 3
fi

#inizializzo variabili
stringa=$1
dir=$2
num=$3

#agginuta della directory corrente alla variabile $PATH
cur_path="$(pwd)"
PATH=$PATH:$cur_path
export PATH

#creazione del file max_counter.tmp e max_dirname.tmp
touch "./max_counter.tmp"
touch "./max_dirname.tmp"

#Chiamata a cercaStringa.sh <parametri> e stampa del risultato
sh "2_cercaStringa.sh" "$stringa" "$dir" "$num" "$cur_path"

echo "$(cat "./max_dirname.tmp")"

rm "./max_counter.tmp"
rm "./max_dirname.tmp"