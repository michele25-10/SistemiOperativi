#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*Variabili globali --> usate per condividere variabili tra il corpo principale dal processo e 
                        handler di segnali ma non possono essere dichiarate in modo normale, bensì
                        di tipo "static volatile sig_atomic_t*/
static volatile sig_atomic_t count = 0; 

void gestore(int signo){
    printf( "\nIl processo %d ha eseguito %d iterazione per il segnale", getpid(), count, signo ); 
    exit(0);  
}

int main(int argc, char** argv){
    int nSec; int nFigli; 
    int *pid; int status;
    
    if(argc != 3){
        fprintf(stderr, "itercount Nfigli Nsec"); 
        exit(1); 
    }

    if((nFigli = atoi(argv[1])) < 0){
        fprintf(stderr, "Numero figli deve essere un numero"); 
        exit(2); 
    }
    
    if((nSec = atoi(argv[2])) < 0){
        fprintf(stderr, "Numero di secondi deve essere un numero"); 
        exit(3); 
    }

    //allocazione dinamica dell'array
    pid = (int*) malloc(nFigli * sizeof(int));  

    for(int i = 0; i < nFigli; i++){
        pid[i] = fork(); 
        if(pid[i] < 0){
            fprintf(stderr, "Errore creazione del pid"); 
            exit(4); 
        } else if(pid[i]  == 0) {
            struct sigaction sa;  
            sigemptyset(&sa.sa_mask); 
            sa.sa_flags = 0; 
            sa.sa_handler = gestore;

            if(sigaction(SIGUSR1, &sa, NULL) < 0){
                perror("Errore nella ricezione del segnale"); 
                exit(-3); 
            }

            while(1){
                sleep(1); 
                count++; 
            }
        }
    }

    //PADRE
    sleep(nSec); 
    for(int i = 0; i < nFigli; i++){
        kill(pid[i], SIGUSR1); 
        /*Qualora avvessi fatto una sola kill(0, SIGUSR1) al gruppo di processi, 
        avendo cura di far ignorare al padre il segnale in questione
        sigignore(SIGUSR1)*/
    }

    //Libero lo spazio di memoria
    free(pid); 

    for(int i = 0; i < nFigli; i++){
        wait(&status); 
    }

    printf("\n"); 

    return 0; 
}

