#!/bin/sh

dir=$1
cd $dir

for f in *.txt
do
    if (test -r $f); then
        if (test "$(head -n 1 $f | grep -c "giochi")" -gt 0); then 
            echo "$dir/$f" >> $HOME/trovato.txt
            count="$(wc -l $f | cut -d " " -f 1)"
            if (test $count -gt $(cat /tmp/max_quantita)); then
                echo $count > /tmp/max_quantita
                echo "$dir/$f" > /tmp/max_file
            fi
        fi    
    fi
done

for d in *
do
    if (test -d $d); then
        trova_file_giochi_ric.sh "$dir/$d"
    fi
done