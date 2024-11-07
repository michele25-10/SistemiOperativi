#!/bin/sh

if (test $# -ne 1); then
    echo "trova_errori.sh <dir>"
    exit 1
fi

case "$1" in 
    /*);; 
    *) 
        echo "dir deve essere una directory assoluta"
        exit 2;;
esac

if (test ! -d $1); then
    echo "Directory inesistente"
    exit 3
fi

PATH=$PATH:"$(pwd)"
export PATH

ERROR_PATH="$HOME/error.txt"; export ERROR_PATH
> $ERROR_PATH

WARNING_PATH="$HOME/warning.txt"; export WARNING_PATH
> $WARNING_PATH

DIR_MAX_TMP="$(pwd)/max_dir.tmp"; export DIR_MAX_TMP
> $DIR_MAX_TMP

COUNTER_MAX_TMP="$(pwd)/counter_max.tmp"; export COUNTER_MAX_TMP
echo 0 > $COUNTER_MAX_TMP

sh "trova_errori_ric.sh" "$1"

echo "La directory con il maggior numero di errori è: $(cat "$DIR_MAX_TMP") con $(cat "$COUNTER_MAX_TMP") errori"

rm $DIR_MAX_TMP
rm $COUNTER_MAX_TMP
