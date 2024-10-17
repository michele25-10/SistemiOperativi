#! /bin/bash sh 

#controllo input
if (test $# -ne 1); then
    echo "sh 3_cancella_versioni_precedenti.sh <dirRelativa>"
    exit 1
fi

if (test ! -d $1); then
    echo "La directory indicata non esiste"
    exit 2
fi

#aggiunta della directory alla variabile $path
cur_path="$(pwd)"
PATH=$PATH:$cur_path
FATHER_PATH=$cur_path
export PATH
export FATHER_PATH

#creazione del file max counter con valore 0 e max_dirname.tmp
echo 0 > "./max_counter.tmp"
touch "./max_dirname.tmp"

sh "./3_cancella_versioni_preliminari_aux.sh" "$1"

echo "$(cat "./max_dirname.tmp")"

#rm "./max_counter.tmp"
#rm "./max_dirname.tmp"