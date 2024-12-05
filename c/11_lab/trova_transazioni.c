#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define DIM 32

void ctrlc(int signo){
    printf( "\nProgramma terminato"); 
    exit(0);  
}

int main(int argc, char **argv){
    char cliente[DIM]; 
    char data[5]; 
    int n; 
    int p1p2[2]; int p2p3[2]; 
    int pid1, pid2, pid3; 
    int status; 
    
    if(argc != 2){
        fprintf(stderr, "\nErrore del numero di argomenti\n"); 
        exit(1); 
    }

    if(argv[1][0] != '/'){
        fprintf(stderr, "\nLa directory non è assoluta\n"); 
        exit(2);  
    }

    DIR* dir = opendir(argv[1]);
    if(dir == NULL){
        fprintf(stderr, "\nLa directory non è assoluta\n"); 
        exit(1); 
    } 
    //fclose(dir); 

    //Gestione del segnale
    signal(SIGINT, ctrlc); 

    while(1){
        printf("\nInserisci il codice del cliente: "); 
        scanf("%s", cliente); 

        printf("\nInserisci la data di interesse (MMGG): "); 
        scanf("%s", data); 

        printf("\nInserisci il numero di righe di interesse: "); 
        scanf("%d", &n);

        if(pipe(p1p2) < 0){
            perror("Errore creazione pipe p1p2"); 
            exit(3); 
        }

        if(pipe(p2p3) < 0){
            perror("Errore creazione pipe p2p3"); 
            exit(4); 
        }

        //Controllo esistenza del file
        char filePath[128]; 
        sprintf(filePath, "%s/%s.tran", argv[1], data); 
        int fd = open(filePath, O_RDONLY); 
        if(fd < 0){
            perror("File inesistente errore apertura file"); 
            exit(5); 
        }
        close(fd); 

        pid1 = fork(); 
        if(pid1 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p1p2[0]); 
            close(1);
            dup(p1p2[1]); 
            close(p1p2[1]); 

            execlp("grep", "grep", cliente, filePath, (char*)0); 
            perror("Errore execlp"); 

            exit(10); 
        }
        if(pid1 < 0){
            perror("Errore creazione pid1"); 
            exit(11); 
        }

        pid2 = fork(); 
        if(pid2 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p1p2[1]); 

            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]); 

            close(p2p3[0]); 

            close(1); 
            dup(p2p3[1]); 
            close(p2p3[1]); 

            execlp("sort", "sort", "-n", (char*)0); 
            perror("Errore execlp"); 

            exit(11); 
        } 
        if(pid2 < 0){
            perror("Errore creazione pid2"); 
            exit(11); 
        }

        //chiudo le pipe che non servono più a pid3
        close(p1p2[0]); 
        close(p1p2[1]); 

        pid3 = fork(); 
        if(pid3 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p2p3[1]); 
            
            close(0); 
            dup(p2p3[0]); 
            close(p2p3[0]); 

            close(1); 
            //apro il file
            int fd = open("importi_massimi.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666); 
            if(fd < 0){
                perror("Errore creazione file"); 
                exit(11); 
            }

            char righe[4]; 
            sprintf(righe, "%d", &n);  

            execlp("head", "head", "-n", righe, (char*)0); 
            perror("Errore execlp"); 

            exit(12); 
        }   
        if(pid3 < 0){
            perror("Errore creazione pid3"); 
            exit(11); 
        }
        
        close(p2p3[0]); 
        close(p2p3[1]); 

        wait(&status); 
        wait(&status); 
        wait(&status); 

    }

}