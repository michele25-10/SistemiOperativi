#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DIM 1024
#define MAX_PROC 15

int main(int argc, char **argv){
    char matricola[DIM]; 
    int p1p2[2]; 
    int pid1; int pid2; int status; 


    if(argc != 2){
        fprintf(stderr, "\nvisualizza_messaggio <num_fork>\n"); 
        exit(1); 
    }

    if(argv[1][0] != '/'){
        fprintf(stderr, "Non hai inserito un percorso assoluto del file\n"); 
        exit(2); 
    }

    //controllo che il file esista
    int fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        fprintf(stderr, "Errore apretura file %s", argv[1]); 
        exit(3); 
    }
    close(fd); 


    printf("\nInserisci la matricola da cercare: "); 
    scanf("%s", matricola); 

    while(strcmp(matricola, "fine") != 0){
        if(pipe(p1p2) < 0){
            exit(5); 
            perror("Errore creazione pipe"); 
        }

        pid1 = fork(); 
        if(pid1 < 0){
            perror("Errore creazione del figlio"); 
            exit(4); 
        } else if(pid1 == 0){
            close(p1p2[0]); //chiudo canale lettura pipe
            
            //redireziono l'output
            close(1);
            dup(p1p2[1]); 
            close(p1p2[1]);  
            
            execlp("grep", "grep", matricola, argv[1], (char*)0); 
            perror("Errore della grep"); 
            exit(6); 
        }

        //PADRE
        pid2 = fork(); 
        if(pid2 < 0){
            exit(7); 
            perror("Errore creazione pipe"); 
        } else if(pid2 == 0){
            close(p1p2[1]); //chiudo canale di scrittura
            
            //ridireziono lo stdin 
            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]); 

            execlp("wc", "wc", "-l", (char*)0); 
            perror("Errore della ws"); 
            exit(7); 
        }

        // chiudo la pipe
        close(p1p2[0]); 
        close(p1p2[1]); 

        //attendo terminazione processo 1 e 2
        wait(&status); 
        wait(&status); 


        printf("\nInserisci la matricola da cercare: "); 
        scanf("%s", matricola); 
    }
    
    return 0; 
}