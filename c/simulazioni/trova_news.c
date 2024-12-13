#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define DIM_DATA 9
#define DIM_ARG 32
#define DIM_PATH 256
#define DIM_BUFFER 1024

static volatile sig_atomic_t count = 0; 

void handler_sigint(){
    printf("\nIl programma ha terminato con: %d\n", count); 
    exit(0); 
}

int main(int argc, char**argv){
    int pid1, pid2, status; 
    int p1p2[2]; int p2p0[2]; 
    int n_read; 
    char path[DIM_PATH]; 
    char data[DIM_DATA]; char argomento[DIM_ARG]; 
    char buffer[DIM_BUFFER]; 

    if(argc != 2){
        printf("\nErrore nel numero di argomenti\n"); 
        exit(1); 
    }

    if(argv[1][0] != '/'){
        printf("\nErrore il percorso del file non è assoluto\n"); 
        exit(2); 
    }

    int fd = open(argv[1], __O_DIRECTORY); 
    if(fd < 0){
        printf("\nDir non esistente\n"); 
        exit(3); 
    }
    close(fd); 

    signal(SIGINT, handler_sigint); 

    while(1){
        printf("\nInserisci la data di interesse: \n");
        scanf("%s", data); 
        if(strcmp(data, "fine") == 0){
            break; 
        }

        printf("\nInserisci argomento: \n"); 
        scanf("%s", argomento); 
        if(strcmp(argomento, "fine") == 0){
            break;
        }

        sprintf(path, "%s/%s.txt", argv[1], data); 
        fd = open(path, O_RDONLY); 
        if(fd < 0){
            printf("\nErrore apertura file\n"); 
            continue; 
        }
        close(fd); 

        if(pipe(p1p2)<0){
            printf("\nErrore creazione pipe p1p2\n"); 
            exit(4); 
        }

        pid1 = fork(); 
        if(pid1 < 0){
            printf("\nErrore creazione pid1\n"); 
            exit(5); 
        }
        if(pid1 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p1p2[0]); 

            close(1); 
            dup(p1p2[1]); 
            close(p1p2[1]);

            execlp("grep", "grep", argomento, path, (char*)0); 
            perror("\nErrore grep\n");

            exit(6); 
        }

        close(p1p2[1]); 

        if(pipe(p2p0)<0){
            printf("\nErrore creazione pipe p2p0\n"); 
            exit(7); 
        }

        pid2 = fork(); 
        if(pid2 < 0){
            printf("\nErrore creazione pid2\n"); 
            exit(8); 
        }
        if(pid2 == 0){
            signal(SIGINT, SIG_DFL);

            close(p2p0[0]); 

            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]);

            close(1); 
            dup(p2p0[1]); 
            close(p2p0[1]);

            execlp("sort", "sort", "-k", "1", "-t", ",", "-n", "-r", (char*)0); 
            perror("Errore sort");  
            exit(9); 
        }

        close(p1p2[0]); 
        close(p2p0[1]);

        wait(&status); 
        wait(&status); 

        n_read = read(p2p0[0], buffer, DIM_BUFFER); 
        if(n_read < 0){
            printf("\nErrore fase di lettura dalla pipe nel padre\n"); 
            exit(10); 
        }
        buffer[n_read] = '\0';
        printf("\nTitoli trovati nel sistema per la data %s: \n%s", data, buffer);   

        close(p2p0[0]); 
        count++; 
    }

    return 0; 
}