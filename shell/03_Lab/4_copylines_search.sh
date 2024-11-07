#!/bin/bash sh

dir=$1
str=$2
echo $dir 
echo $*
shift 

cd $dir

i=0

for f in *; do
    if (test -f $f); then
        for args in $*; do 
            if (test $f = $args && grep -q $str $f); then
                echo $dir/$f
                i=$(expr $i + 1)
                cp "$f" "$FATHER_PATH/tmp/$f"
            fi
        done
    elif (test -d $f); then
        sh "$FATHER_PATH/4_copylines_search.sh" "$dir/$list_file"
    fi
done

echo $i >$FATHER_PATH/tmp_counter.tmp
