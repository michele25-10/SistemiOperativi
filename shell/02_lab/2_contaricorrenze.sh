#!/bin/bash 

if test $# -lt 2 
    then 
    echo "Il primo argomento deve contenere il filename e il secondo la parola da cercare"
    exit 1   
fi

filename="$1"
if test ! -f $filename
    then
    echo "Il file deve esistere"
    exit 2
fi 
shift

for i in $* 
do
    echo "$i - ricorrenze $(grep $i $filename| wc -l)"
done

exit 0