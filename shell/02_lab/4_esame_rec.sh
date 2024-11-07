#!/bin/bash sh
currentPath="$(pwd)/4_esame_rec.sh"
PATH=$PATH:$currentPath
export PATH

estensione="$1"
dir1="$2"
dir2="$3"

cd "$dir1"
for i in *
    do 
    if echo "$i" | grep -q "$estensione" 
        then
        echo "file trovato"
        if test ! -f "$dir2/$i"
            then
                echo "copia $i"
                cp "$i" "$dir2"
        fi
    elif test -d $i
        then
        sh 4_esame_rec.sh "$estensione" "$dir1/$i" "$dir2"
    fi
done
