#! user/bin/env sh

currentPath="$(pwd)"

if test $# -ne 2
    then
    echo "sh nomescript <nomedirectory> <nomefile>"
    exit 1    
fi

dir="$1"
file="$2"

echo $dir e $file

if test ! -d $dir
   then
    echo "Il percorso assoluto della directory deve esistere"
    exit 2 
fi

cd $dir

for f in *
    do  
    if test -f $f
        then
        if test $f = $file
            then
            echo "trovato $(pwd)/$file"
        fi
        elif test -d $f 
            then
            sh "$currentPath/3_trova.sh" "$dir/$f" "$file"
        fi
done