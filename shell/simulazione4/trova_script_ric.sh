#!/bin/sh

dir=$1
cd "$dir"

counter=0

for f in *.sh
do 
    if (test -r $f -a -x $f); then
        if (test "$(head -n 1 $f | grep -c "#!/bin/bash" $f)" -gt 0); then
            counter=$(expr $counter + 1)
            echo "$dir/$f" >> "$HOME/script.txt"
        fi
    fi
done

if (test "$counter" -gt "$(cat "/tmp/max_quantita")"); then
    echo "$dir" > /tmp/max_directory
    echo "$counter" > /tmp/max_quantita
fi

for d in *
do 
    if (test -d $d); then
        trova_script_ric.sh "$dir/$d"
    fi
done


