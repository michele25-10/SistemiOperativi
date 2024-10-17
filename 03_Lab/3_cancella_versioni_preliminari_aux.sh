#!/bin/bash sh 

cd $1

i=0
echo $1

for f in *; do
    if test "$f" = *txt; then
        if (test -w "$f"); then
        echo $f sono dentro
        version="$(head $f -n 1 | cut -d ',' -f3)"
        if ( test $version = "preliminare"); then
            i=$(expr $i + 1)
            #rm $f
            row_max_counter="$(cat $FATHER_PATH/max_counter.tmp)"
            if (test $i -gt $row_max_counter); then
                echo "i >  $row_max_counter"
                echo "$i" > "$FATHER_PATH/max_counter.tmp"
                echo "$1" > "$FATHER_PATH/max_dirname.tmp"
            fi
        fi
        fi
    elif test -d $f; then
        sh "$FATHER_PATH/3_cancella_versioni_preliminari_aux.sh" "$(pwd)/$f"
    else
    echo elseeeee
    fi
done