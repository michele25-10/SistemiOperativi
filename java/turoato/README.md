# MAIN

- istanzio oggetti comuni, thread e pipedStream
- aspetto comunicazioni o fine esecuzione
- join() --> attendo chiusura processo in un try catch

# Oggetti thread

- creati da MAIN
- eseguono qualcosa
- comunicano col MAIN quando hanno finito

# Oggetti comuni

- istanziati da MAIN, non messi su thread
- offrono metodi pubblici syncronizhed o variabili Atomic per variabili shared
