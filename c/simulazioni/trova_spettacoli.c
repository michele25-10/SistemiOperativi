//Simulazione 2021 luglio 09 durata 1h e 30

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
#define DIM_BUFF 1024
#define DIM_PATH 128 

static volatile sig_atomic_t count = 0; 

void handler_sigint(){
    printf("\nNumero di richieste servite: %d\n", count); 
    exit(0); 
}

int main(int argc, char**argv){
    int pid1, pid2, pid3, status; 
    int p1p2[2], p2p3[2], p3p0[2]; 
    char nome[DIM], path[DIM_PATH]; 
    int N; 
    int n_read;
    int fd;  
    char buffer[DIM_BUFF]; 

    if(argc != 2){
        printf("\nErrore numero di argomenti\n"); 
        exit(1); 
    }

    const char* home = getenv("HOME"); 
    if(home == NULL){
        printf("\nErrore get variabile ambiente home\n"); 
        exit(1); 
    }

    sprintf(path, "%s/%s.txt", home, argv[1]); 
     
    fd = open(path, O_RDONLY);  
    if(fd < 0){
        printf("\nFile non esistente prova inserendo un altro nome\n");
        exit(2);  
    }
    close(fd);
    
    signal(SIGINT, handler_sigint); 

    while(1){
        printf("\n\nInserisci il nome dello spettacolo: \n"); 
        scanf("%s", nome); 
        if(strcmp(nome, "0") == 0){
            break; 
        }

        printf("\nInserisci il numero di campi di interesse: \n");
        scanf("%d", &N); 
        if(N == 0){
            break; 
        }
        if(N > 999 || N < 0){
            printf("\nN deve essere compreso tra 0 e 999\n"); 
            continue; 
        }

        if(pipe(p1p2) < 0){
            printf("\nErrore creazione pipe: \n"); 
            exit(3); 
        }

        pid1 = fork(); 
        if(pid1 < 0){
            printf("\nErrore creazione pid1\n"); 
            exit(4); 
        }
        if(pid1 == 0){  
            signal(SIGINT, SIG_DFL);
            
            close(p1p2[0]); 
            
            close(1); 
            dup(p1p2[1]); 
            close(p1p2[1]); 
        
            execlp("grep", "grep", nome, path, (char*)0); 
            perror("Execlp grep"); 
            exit(5); 
        }

        close(p1p2[1]); 

        if(pipe(p2p3) < 0){
            printf("\nErrore creazione pipe p2p3\n"); 
            exit(6); 
        }

        pid2 = fork(); 
        if(pid2 < 0){
            printf("\nErrore creazione pid2\n"); 
            exit(7); 
        }
        if(pid2 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p2p3[0]); 
            
            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]); 

            close(1); 
            dup(p2p3[1]); 
            close(p2p3[1]); 

            execlp("sort", "sort", "-n", "-r", (char*)0); 
            perror("\nErrore execlp sort\n"); 
            exit(8); 
        }

        close(p1p2[0]); 
        close(p2p3[1]); 

        if(pipe(p3p0) < 0){
            printf("\nErrore creazione pipe p3p0\n"); 
            exit(9); 
        }

        pid3 = fork();
        if(pid3 < 0){
            printf("\nErrore creazione pid3\n"); 
            exit(10); 
        } 
        if(pid3 == 0){
            signal(SIGINT, SIG_DFL); 

            close(p3p0[0]); 

            close(0); 
            dup(p2p3[0]); 
            close(p2p3[0]); 

            close(1); 
            dup(p3p0[1]); 
            close(p3p0[1]); 

            char n_str[4]; 
            sprintf(n_str, "%d", N);

            execlp("head", "head", "-n", n_str, (char*)0); 
            perror("\nErrore execpl head\n"); 
            exit(11); 
        }

        close(p2p3[0]); 
        close(p3p0[1]); 

        wait(&status); 
        wait(&status); 
        wait(&status); 

        if((n_read = read(p3p0[0], buffer, DIM_BUFF)) < 0){
            printf("\nErrore lettura da pipe in p0\n"); 
            exit(12); 
        }
        buffer[n_read] = '\0'; 
        printf("\nRisposta del programma:\n%s\n", buffer); 

        close(p3p0[0]); 

        count++; 
    }

    handler_sigint(); 
    return 0; 
}
