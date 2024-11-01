#!/bin/sh
#trova_dir.sh <dir>

if (test $# -ne 1); then
    echo "trova_dir.sh <dir>"
    exit 1
fi

if (test ! -d $1); then
    echo "La directory da lei inserita non esiste"
    exit 2
fi

case $1 in 
    /*);;
    *)
        echo "Il percorso della directory deve essere assoluto"
        exit 3 
    ;;
esac

PATH=$PATH:"$(pwd)"
export PATH

if (test ! -f "$HOME/script.txt"); then
    echo "" > "$HOME/script.txt"
fi

echo "" > /tmp/max_directory
echo 0 > /tmp/max_quantita

trova_script_ric.sh $1

echo "La directory con il maggior numero di file è contente #!/bin/bash: $(cat /tmp/max_directory))"

rm -f /tmp/max_directory
rm -f /tmp/max_quantita
