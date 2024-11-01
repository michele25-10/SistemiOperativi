#!/bin/sh
#trova_file_giochi.sh <dir>

if (test $# -ne 1); then
    echo "trova_file_giochi.sh <dir>"
    exit 1
fi 

if (test ! -d $1); then
    echo "Directory inesistente"
    exit 2
fi

case $1 in 
    /*);; 
    *)
        echo "Errore il path della directory deve essere assoluto"
        exit 3
    ;;
esac

PATH=$PATH:"$(pwd)"
export PATH

if (test ! -f "$HOME/trovato.txt"); then
    > $HOME/trovato.txt
fi

> "/tmp/max_file"
echo 0 > /tmp/max_quantita

trova_file_giochi_ric.sh "$1"

echo "Il file che contiene più giochi è $(cat /tmp/max_file)"

rm -f /tmp/max_file
rm -f /tmp/max_quantita



