#!/bin/bash

# Verifica che il primo argomento sia una directory valida
cd "$1" || exit 1

i=0
echo "$1"

for f in *; do
    if [[ "$f" == *.txt ]]; then
        if [[ -w "$f" ]]; then
            echo "$f sono dentro"
            version="$(head -n 1 "$f" | cut -d ',' -f3)"
            
            if [[ "$version" == "preliminare" ]]; then
                i=$((i + 1))  # Incrementa i
                row_max_counter="$(cat "$FATHER_PATH/max_counter.tmp")"
                
                if [[ $i -gt $row_max_counter ]]; then
                    echo "i >  $row_max_counter"
                    echo "$i" > "$FATHER_PATH/max_counter.tmp"
                    echo "$1" > "$FATHER_PATH/max_dirname.tmp"
                fi
            fi
        fi
    elif [[ -d "$f" ]]; then
        bash "$FATHER_PATH/3_cancella_versioni_preliminari_aux.sh" "$(pwd)/$f"
    fi
done
