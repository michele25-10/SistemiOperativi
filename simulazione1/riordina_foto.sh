#!/bin/sh

if (test $# -lt 2); then
    echo "riordina_foto.sh <dir_sorgente> <di_destinazione>"
    exit 1
fi

if (test ! -d $1); then
    echo "dir_sorgente: inesistente"
    exit 2
fi

if (test ! -d $2); then
    echo "dir_destinazione: insesistente"
    exit 3
fi

srg=$1
dst=$2

PATH=$PATH:"$(pwd)"
export PATH

cd "$srg"
srg_abs="$(pwd)"
echo SORGENTE $srg_abs

cd "$dst"
dst_abs="$(pwd)"
echo DESTINAZIONE $dst_abs

if (test ! -d "duplicati"); then
    mkdir "duplicati"
fi

ricorsivo.sh "$srg_abs" "$dst_abs"

echo "FILE non duplicati $(ls $dst_abs)"
echo "FILE duplicati $(ls "$dst_abs/duplicati")"

#sh riordina_foto.sh /home/studente/Scrivania/SistemiOperativi/prova /home/studente/Scrivania/SistemiOperativi/prova2