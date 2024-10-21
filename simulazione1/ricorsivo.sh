srg=$1
dst=$2

cd $srg

for file in *.jpg
    do
    if (test -r $file); then 
        if (test -f $dst/$file); then
            echo "$dst/file" sposto in duplicati
            mv "$srg/$file" "$dst/duplicati/$file"
        else
            mv "$srg/$file" "$dst/$file"
        fi
    fi
done

for directory in *
do
    if (test -d $directory); then
        sh "$FATHER_PATH/ricorsivo.sh" "$srg/$directory" "$dst"
    fi
done
