#!/bin/sh

dir=$1
file_output=$2

cd "$dir"

count=0
categoria=""

echo "" > /tmp/lista_articoli

for f in *.log
do
    if (test -r $f); then
        count="$(expr $count + $(grep -c "venduto" $f))"
        echo "$(grep "venduto" $f)" >> /tmp/lista_articoli
        if (test ! -n "$categoria"); then
            categoria=$(cut $f -d "," -f 3 -z)
        fi
    fi
done


if (test $count -gt "$(cat /tmp/max_quantita)"); then
    echo "$count" > /tmp/max_quantita
    echo "$categoria" > /tmp/max_categoria
    echo "$(cat /tmp/lista_articoli)" > $file_output
fi

for d in *
do 
    if (test -d $d); then
        articoli_ric.sh "$dir/$d" "$file_output"
    fi
done

