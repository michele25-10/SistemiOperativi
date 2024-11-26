#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DIM 100

static volatile sig_atomic_t count = 0; 

void ctrlc(int signo){
    printf( "\nIl processo %d ha eseguito %d iterazione per il segnale", getpid(), count ); 
    exit(0);  
}

void syncronize(int signo){
    printf("\nArrivato il segnale di avvio pid 1: "); 
    exit(0); 
}

int main(int argc, char**argv){
    int pid1; int pid2; int pid3; int status; 
    int p1p2[2]; int p2p3[2]; 
    char uid[5]; char nRaw[2]; 

    if(argc != 2){
        fprintf(stderr, "\nvisualizza_messaggio <num_fork>\n"); 
        exit(1); 
    }

    if(argv[1][0] != '/'){
        fprintf(stderr, "Non hai inserito un percorso assoluto del file\n"); 
        exit(2); 
    }

    struct sigaction ctrl;  
    sigemptyset(&ctrl.sa_mask); 
    ctrl.sa_flags = 0; 
    ctrl.sa_handler = ctrlc;
            
    if(sigaction(SIGINT, &ctrl, NULL) < 0){
        perror("Errore ricezione segnale"); 
        exit(6); 
    }

    while(1){
        printf("Inserisci il codice dell'utente: "); 
        scanf("%s", uid); 
        printf("\nInserisci il numero di raw: "); 
        scanf("%s", nRaw); 

        //Nome del file
        char filename[DIM]; 
        strcpy(filename, argv[1]);
        strcat(filename, "/"); 
        strcat(filename, uid); 
        strcat(filename, ".txt") ; 

        //creo le pipe
        if(pipe(p1p2)<0 || pipe(p2p3)){
            perror("creazione pipe"); 
            exit(3); 
        }

        //primo figlio
        pid1 = fork(); 
        if(pid1 == 0){
            struct sigaction sa;  
            sigemptyset(&sa.sa_mask); 
            sa.sa_flags = 0; 
            sa.sa_handler = syncronize;
            
            if(sigaction(SIGUSR1, &sa, NULL) < 0){
                perror("Errore ricezione segnale"); 
                exit(6); 
            }

            pause(); 

            close(p1p2[0]); //chiudo canale lettura
            
            //ridirezione dell'output
            close(1); 
            dup(p1p2[1]); 
            close(p1p2[1]); 

            execlp("sort", "sort", filename, (char*)0); 
            perror("Exec sort"); 
            exit(-7); 
        } else if(pid1 < 0){
            perror("Errore creazione figlio 1"); 
            exit(4); 
        }

        //Secondo figlio
        pid2 = fork(); 
        if(pid2 == 0){
            close(p1p2[1]); //chiudo canale di scrittura p1p2 
            
            //ridireziono input
            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]); 

            close(p2p3[0]); //chiudo canale di lettura p2p3  
            
            //ridirezione output
            close(1); 
            dup(p2p3[1]);
            close(p2p3[1]); 

            execlp("grep", "grep", "NON RESTITUITO", filename, (char*)0);
            perror("Errore grep"); 
            exit(-9);  

        } else if(pid2 < 0){
            perror("Errore creazione figlio 2"); 
            exit(5); 
        }

        //terzo figlio
        pid3 = fork(); 
        if(pid3 == 0){
            close(p2p3[1]); //chiudo il canale di output

            //ridirezione input
            close(0); 
            dup(p2p3[0]); 
            close(p2p3[0]); 

            execlp("head", "head", "-n", nRaw, (char*)0);
            perror("Errore head"), 
            exit(-10);
        } else if(pid3 < 0){
            perror("Errore creazione figlio 3"); 
            exit(6); 
        }

        close(p1p2[0]); 
        close(p1p2[1]); 
        close(p2p3[0]); 
        close(p2p3[1]);

        kill(pid1, SIGUSR1);

        wait(&status);
        wait(&status);
        wait(&status);

        count++; 
    }

    return 0; 
}