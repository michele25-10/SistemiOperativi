#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv){
    int pid, fd, status, num, file_exist; 
    char filename[32]; 
    
    if(argc != 2){
        fprintf(stderr, "3_conteggio <num>"); 
        exit(1); 
    }

    if((num = atoi(argv[1]))<= 0){
        fprintf(stderr, "Il secondo argomento deve essere numerico e non %s", argv[1]); 
        exit(2); 
    }

    printf("\nSe inserisci la stringa \"fine\""); 

    printf("\nInserisci il nome del file:"); 
    scanf("%s", filename); 

    while(strcmp(filename, "fine\n") != 0){
        file_exist = 1;  
        
        //Controllo che il file esista
        fd = open(filename, O_RDONLY); 
        if(fd < 0){
            //L'open potrebbe fallire anche in caso di mancanza di permessi
            if(errno == ENOENT){
                //il file non esiste
                printf("\nIl file %s non esiste", filename); 
                file_exist = 0; 
            } else {
                //Tutti gli altri casi di errore
                perror("open"); 
                exit(3);
            }
        }
        close(fd); 

        if(file_exist){
            pid = fork(); 
            if(pid < 0){
                fprintf(stderr, "Errore creazione figlio"); 
                exit(4); 
            } else if(pid == 0){ //FIGLIO
                printf("\nANTEPRIMA"); 

                execlp("head", "head", "-n", argv[1], filename, (char*)0); 
            
                //Solo se l'exec non va a buon fine allora 
                perror("grep"); 

                //Se l'exec va a buon fine faccio l'exit del figlio che viene rilevata dal padre
                exit(5); 
            } else { //PADRE
                wait(&status); 
            }
        }
        
        printf("\nInserisci il nome del file:"); 
        scanf("%s", filename); 
    }

    return 0;
}