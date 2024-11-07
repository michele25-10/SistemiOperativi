#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    int pid, fd, status, k; 

    //Controllo argomenti
    if (argc < 3){
        fprintf(stderr, "copy_input <nome_file>");
        exit(1); 
    }

    //Se il file non esiste lo creo se è pieno lo svuoto
    fd = open("conteggio.txt", O_CREAT|O_TRUNC|O_WRONLY , 0644); 
    if(fd < 0){
        fprintf(stderr, "Errore apretura file %s", argv[1]); 
        exit(2); 
    }
    close(fd); 

    for(k = 2; k < argc; k++){
        int pid = fork(); 
        if(pid == 0){ //FIGLIO
            char log[256]; 

            //Apro il file in append, non ho bisogno di specificare i permessi (non sto creando)
            int fd = open(argv[1], O_WRONLY|O_APPEND); 
            if(fd < 0){
                fprintf(stderr, "Errore apretura file %s", argv[1]); 
                return -1; 
            }

            //Creo la stringa di output che inserisce la stringa
            // <nome_file> <stringa> nel mio array di char log
            sprintf(log, "%s %s\n", argv[1], argv[k]); 

            //Scrivo nel file argv[1]
            write(fd, log, strlen(log)); 
            
            close(fd); 
            
            printf("\nNum di righe in cui compare la stringa %s: ", argv[1]); 
            //(char*)0 è obbligatorio, simboleggia la fine degli argomenti del comando eseguito
            execlp("grep" "grep" "-c", argv[k], argv[1], (char *)0); 

            perror("exec"); 
            exit(4); 
        } else if (pid < 0){
            fprintf(stderr, "\nErrore creazione figlio"); 
            exit(3); 
        }
    }

    //Se arrivo qui sono sicuramente il padre
    for(k=2; k<argc; k++){
        //Attendo la terminazione di uno dei figli
        wait(&status); 
    }

    return 0;
}