#!/bin/bash sh
if test $# -ne 3
    then
    echo "esame.sh <estensione> <dir1> <dir2>"
    exit 1
fi

estensione=$1
is_point={$1:0:1}
if test $is_point = "."
    then 
    echo "L'estensione deve cominciare con il punto"
    exit 2
fi

dir1=$2
if test ! -d $dir1
    then
    echo "dir1 deve essere un percorso assoluto esistente"
    exit 3
fi

dir2=$3
if test ! -d $dir2
    then
    echo "dir2 deve essere un percorso assoluto esistente"
    exit 4
fi

sh 4_esame_rec.sh "$estensione" "$dir1" "$dir2"
exit 0