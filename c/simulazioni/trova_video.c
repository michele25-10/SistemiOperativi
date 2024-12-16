//simulazione esame 19 Giugno 2020  durata 1:15

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define DIM_PATH 256
#define DIM 32
#define DIM_DATA 7

volatile sig_atomic_t n_req = 0; 

void handler_sigint(){
    printf("\nIl numero di richieste servite è: %d\n", n_req);
    exit(0); 
}

int main(int argc, char**argv){
    int pid1, pid2, status; 
    int p1p2[2]; 
    char categoria[DIM], data[DIM_DATA], path[DIM_PATH]; 
    int fd; 

    if(argc != 2){
        printf("\nIl numero di argomento è errato\n"); 
        exit(1); 
    }

    if(argv[1][0] == '/'){
        printf("\nIl percorso della directory deve essere relativo\n"); 
        exit(2); 
    }

    fd = open(argv[1], __O_DIRECTORY);
    if(fd < 0){
        printf("\nDirectory non esistente, errore!\n"); 
        exit(3); 
    }
    close(fd); 

    signal(SIGINT, handler_sigint); 

    while(1){
        //prendo le informazioni utili per la ricerca in input
        printf("\n\nInserisci la data:\n"); 
        scanf("%s", data); 
        printf("\nInserisci la categoria:\n"); 
        scanf("%s", categoria); 

        sprintf(path,"%s/%s.txt", argv[1], data); 
        fd = open(path, O_RDONLY); 
        if(fd < 0){
            printf("File inesistente"); 
            continue;  
        }
        close(fd); 

        if(pipe(p1p2)<0){
            perror("\nErrore creazione della pipe\n"); 
            exit(4); 
        }

        pid1 = fork(); 
        if(pid1 < 0){
            printf("\nErrore creazione del pid1\n"); 
            exit(5); 
        }
        if(pid1 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p1p2[0]); 
            
            close(1); 
            dup(p1p2[1]); 
            close(p1p2[1]); 
        
            execlp("grep", "grep", categoria, path, (char*)0); 
            perror("Errore nella grep"); 
            exit(6); 
        }

        close(p1p2[1]); 

        pid2 = fork(); 
        if(pid2 < 0){
            printf("\nErrore creazione del pid2\n"); 
            exit(7); 
        }
        if(pid2 == 0){
            signal(SIGINT, SIG_DFL); 
            
            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]); 
        
            execlp("sort", "sort", "-n", "-r", "-k", "1", "-t", ",", (char*)0); 
            perror("Errore nella sort"); 
            exit(6); 
        }

        close(p1p2[0]); 

        wait(&status); 
        wait(&status); 

        n_req++; 
    }

    return 0; 
}