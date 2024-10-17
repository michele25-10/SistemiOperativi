#!bin/bash sh

father_path=$4

#Entro nella directory
cd $2

#Creo file counter.tmp
touch "./counter.tmp"

for f in *; do
    if (test -f $f); then
        if (test -r $f); then
            if (test $(grep -c "$1" "$f") -ge $3); then
                echo "riga" >> "./counter.tmp"
                n_counter="$(wc -l "./counter.tmp" | cut -d ' ' -f1)"
                n_max_counter="$(wc -l "$father_path/max_counter.tmp" | cut -d ' ' -f1)"
                if (test $n_counter -gt $n_max_counter); then
                    cat "./counter.tmp" >"$father_path/max_counter.tmp"
                    echo "$2" > "$father_path/max_dirname.tmp"
                fi
            fi

        fi
    else
        sh "$father_path/2_cercaStringa.sh" "$1" "$2/$f" "$3" "$father_path"
    fi
done

rm "./counter.tmp"
