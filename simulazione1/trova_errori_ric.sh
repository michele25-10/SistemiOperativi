#!/bin/sh

dir=$1
cd "$dir"

counter=0

for file in *.log; do
    if (test -f $file -a -r $file); then
        (grep "error" $file | cut -d "," -f 2,1,6) >> $ERROR_PATH
        (grep "warning" $file | cut -d "," -f 2,1,6) >> $WARNING_PATH
        counter="$(expr $counter + "$(grep "error" $file | wc -l)")"
    fi
done

max_counter="$(cat $COUNTER_MAX_TMP)"

if (test $counter -gt $max_counter ); then
    echo "$counter" > $COUNTER_MAX_TMP
    echo "$(pwd)" > $DIR_MAX_TMP
fi

echo $PATH

for directory in *; do
    if (test -d "$directory" -a -r "$directory"); then 
        trova_error_ric.sh "$dir"
    fi
done