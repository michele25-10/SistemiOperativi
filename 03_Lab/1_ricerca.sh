#!bin/bash sh

num=$1
dir_sor=$2
dir_dest=$3
shift 3

#Entrata nella directory passata come parametro
cd $dir_sor

for i in $*; do
    for f in *.$i; do
        if (test -f $f); then
            if (test -r $f && test "$(wc -l $f | cut -d ' ' -f1)" -gt $num); then
                echo "riga" >>"./counter.tmp"
                cp "$dir_sor/$f" "$dir_dest/"
            fi
        else
            if (test -x $f); then
                sh "./1_ricerca.sh" "$num" "$dir_sor/$f" "$dir_dest" $*
            fi
        fi
    done
done
