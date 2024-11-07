#!/bin/sh
# trova_ric.sh dir cod_parte num

cd "$1"
codice="$2"
num_pezzi="$3"

for f in *.txt
do 
    if (test -r $f ); then
        pezzi_temp=$(grep "$codice" "$f" | cut -d "," -f 4)
        forni_temp=$(grep "$codice" "$f" | cut -d "," -f 1)

        if (test $pezzi_temp -ge $(cat /tmp/max_parti)); then
            echo "$pezzi_temp" > /tmp/max_parti
            echo "$forni_temp" > /tmp/max_fornitore
        fi
    fi
done

for d in *
do 
    if ( test -d $d); then
        trova_ric.sh "$(pwd)/$d" "$codice" "$num_pezzi"
    fi
done