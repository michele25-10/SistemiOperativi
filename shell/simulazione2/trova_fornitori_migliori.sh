#!/bin/sh
# trova_fornitore_migiori.sh dir cod_parte num

if (test $# -ne 3); then
    echo "Errore numero argomenti deve essere almeno 3"
    exit 1
fi

case $1 in 
    /*);;
    *)
        echo "Errore arg 1 deve essere una directory assoluta"
        exit 2
        ;;
esac

if (test ! -d $1); then
    echo "Errore, la directory inserita non esiste"
    exit 3
fi

case $3 in 
    *[!0-9]*)
        echo "3 deve essere un numero"
        exit 4
        ;;
    *);;
esac

PATH=$PATH:"$(pwd)"
export PATH

#/dev/null --> fai redirect qui dell'output per ignorare l'esito dei risultati
#2>/dev/null

echo "" > /tmp/max_fornitore
echo 0 > /tmp/max_parti

trova_ric.sh $*

echo "fornitore max = $(cat /tmp/max_fornitore) num pezzi max = $(cat /tmp/max_parti)"

rm -f /tmp/max_fornitore
rm -f /tmp/max_parti

