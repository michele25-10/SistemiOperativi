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
#define DIM_BUFFER 16

void ctrlc(int signo){
    printf( "\nProgramma terminato"); 
    exit(0);  
}

int main(int argc, char **argv){
    char operatore[DIM]; 
    char nome_app[DIM]; 
    int p1p2[2]; int p2p0[2]; 
    int pid1, pid2;  
    int status; 
    
    if(argc != 2){
        fprintf(stderr, "\nErrore del numero di argomenti\n"); 
        exit(1); 
    }

    if(argv[1][0] != '/'){
        fprintf(stderr, "\nLa directory non è assoluta\n"); 
        exit(2);  
    }

    int fd = open(argv[1], __O_DIRECTORY); 
    if(fd < 0){
        perror("Directory inesistente"); 
        exit(1); 
    } 
    close(fd); 

    //Gestione del segnale
    signal(SIGINT, ctrlc); 

    while(1){
        printf("\nInserisci il nome dell'operatore: "); 
        scanf("%s", operatore); 

        printf("\nInserisci il nome dell'app: "); 
        scanf("%s", nome_app); 

        if(pipe(p1p2) < 0){
            perror("Errore creazione pipe p1p2"); 
            exit(3); 
        }

        if(pipe(p2p0) < 0){
            perror("Errore creazione pipe p2p3"); 
            exit(4); 
        }

        //Controllo esistenza del file
        char filePath[128]; 
        sprintf(filePath, "%s/%s.txt", argv[1], operatore); 
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

            execlp("grep", "grep", nome_app, filePath, (char*)0); 
            perror("Errore execlp"); 

            exit(10); 
        }
        if(pid1 < 0){
            perror("Errore creazione pid1"); 
            exit(11); 
        }

        close(p1p2[1]); 

        pid2 = fork(); 
        if(pid2 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p1p2[1]); 

            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]); 

            close(p2p0[0]); 

            close(1); 
            dup(p2p0[1]); 
            close(p2p0[1]); 

            execlp("grep", "grep", "-c", "operativa", (char*)0); 
            perror("Errore execlp"); 

            exit(11); 
        } 
        if(pid2 < 0){
            perror("Errore creazione pid2"); 
            exit(11); 
        }

        //chiudo le pipe che non servono più a pid3
        close(p1p2[0]); 
        close(p2p0[1]); 

        char str[DIM_BUFFER]; 
        int n_read = read(p2p0[0], str, DIM_BUFFER-1);
        str[n_read] = '\0'; 
        close(p2p0[0]); 
        printf("%s\n", str);

        wait(&status); 
        wait(&status); 
    }

    return 0; 

}