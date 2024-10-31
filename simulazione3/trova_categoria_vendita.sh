#!/bin/sh
#trova_categoria_vendita dir lista_articoli

if (test $# -ne 2); then
    echo "Numero di argomenti errato"
    echo "trova_categoria_vendita.sh <dir> <lista_articoli>"
    exit 1
fi

if (test ! -d $1); then
    echo "La directory non esiste"
    exit 3
fi

case $1 in 
    /*);;
    *)
        echo "La directory deve avere un percorso assoluto"
        exit 2
    ;;
esac

PATH=$PATH:"$(pwd)"
export PATH

echo "" > "$2"

echo 0 > /tmp/max_quantita
echo "" > /tmp/max_categoria
echo "" > /tmp/lista_articoli

articoli_ric.sh "$1" "$(pwd)/$2"

echo "categoria: $(cat /tmp/max_categoria) --> $(cat /tmp/max_quantita)"

rm -f /tmp/max_quantita
rm -f /tmp/max_categoria
rm -f /tmp/lista_articoli
