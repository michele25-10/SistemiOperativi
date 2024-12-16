//simulazione esame 24 Novembre 2020  durata 1:15

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
#define DIM_PATH 256
#define DIM_BUFFER 1024

volatile sig_atomic_t qta_byte = 0; 

void sigint_handler(){
    printf("\n\nQuantità di byte letti dal padre: %d\n\n", qta_byte); 
    exit(0);
}

int main(int argc, char**argv){
    int pid1, pid2, status; 
    int p1p2[2], p2p0[2];
    char cognome[DIM], nome_libro[DIM], path[DIM_PATH], buffer[DIM_BUFFER]; 
    int fd;
    int qta_tmp; 

    if(argc != 2){
        printf("\nErrore numero argomenti\n"); 
        exit(1); 
    }

    if(argv[1][0] != '/'){
        printf("\nLa directory non è un percorso assoluto\n"); 
        exit(2); 
    }

    fd = open(argv[1], __O_DIRECTORY); 
    if(fd < 0){
        printf("Errore directory non esistente"); 
        exit(3); 
    }   
    close(fd); 

    signal(SIGINT, sigint_handler); 

    while(1){
        //richiesta dati in input
        printf("\n\nInserisci il nome del libro: \n"); 
        scanf("%s", nome_libro); 
        if(strcmp(nome_libro, "fine") == 0){
            break; 
        }

        printf("\nInserisci il cognome della persona: \n"); 
        scanf("%s", cognome); 
        if(strcmp(cognome, "fine") == 0){
            break; 
        }

        sprintf(path, "%s/%s/%s", argv[1], nome_libro, cognome); 
        printf("PATH: %s", path); 
        fd = open(path, O_RDONLY); 
        if(fd < 0){
            printf("\nErrore file non esistente\n"); 
            continue; 
        }
        close(fd); 

        if(pipe(p1p2)<0){
            perror("\nErrore creazione pipe\n"); 
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

            execlp("sort", "sort", path, "-n", "-t", ",", "-k", "1", (char*)0); 
            perror("\nErrore execlp sort\n"); 
            exit(6); 
        }

        //Chiudo p1p2 scrittura per evitare di chiuderla anche dentro pid2
        close(p1p2[1]); 
        
        if(pipe(p2p0) < 0){
            perror("\nErrore creazione pipe\n"); 
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

            execlp("grep", "grep", "ingresso", (char*)0); 
            perror("\nErrore execlp grep\n"); 
            exit(9); 
        }

        close(p1p2[0]); 
        close(p2p0[1]); 

        //Attendo i due figli
        wait(&status); 
        wait(&status); 

        if((qta_tmp = read(p2p0[0], buffer, DIM_BUFFER)) < 0){
            printf("\nErrore Lettura:\n"); 
            exit(10); 
        } 
        close(p2p0[0]);
        buffer[qta_tmp] = '\0'; 
        qta_byte += qta_tmp; 

        printf("\nRisultato:\n%s", buffer); 
        printf("\n\n\n"); 
    }

    sigint_handler(); 
    return 0; 

}