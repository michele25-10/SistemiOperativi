#! /bin/bash sh 

cd $1

i=0

for f in *; do
    if test "$f" = *.txt -a -w "$f"; then
    echo sono dentro
        version="$(head $f -n 1 | cut -d ',' -f3)"
        if ( test $version = "preliminare"); then
            i=$(expr $i + 1)
            #rm $f
            row_max_counter="$(cat $PATH/max_counter.tmp)"
            echo "$row_max_counter"
            if (test $i -gt $row_max_counter); then
                echo "i >  $row_max_counter"
                echo "$i" > "$FATHER_PATH/max_counter.tmp"
                echo "$1" > "$FATHER_PATH/max_dirname.tmp"
            fi
        fi
    elif (test -d $f); then  
        sh "$FATHER_PATH/3_cancella_versioni_preliminari_aux.sh" "$1/$f"
    fi
done