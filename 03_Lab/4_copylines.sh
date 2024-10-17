#! /bin/bash sh

if (test $# -lt 3); then
    echo "sh copylines <nome_dir_assoluto> <stringa> <nomefile1> ... <nomefileN>"
    exit 1
fi

if (test ! -d $1); then
    echo "Errore directory inesistente"
    exit 2
fi

if (test -z $2); then
    echo "La stringa deve essere popolata"
    exit 3
fi

dir=$1
str=$2
shift 2
echo $*

#for i in $*; do 
    #if (echo $i | grep -q '^[a-z].*'); then 
    #    echo $i contiene nome una lettera diversa dalla a alla z minuscole. 
    #    exit 4
    #fi
#done 

FATHER_PATH="$(pwd)"
export FATHER_PATH

mkdir ./tmp
echo 0 > ./tmp_counter.tmp

sh 4_copylines_search.sh "$dir" "$str" $*

cat ./tmp_counter.tmp
rm ./tmp_counter.tmp