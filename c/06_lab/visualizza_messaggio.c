#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DIM 100
#define MAX_PROC 15

int main(int argc, char **argv){
    int nFork; int status; 
    char buffer[DIM] ; 
   
    if(argc != 2){
        fprintf(stderr, "\nvisualizza_messaggio <num_fork>"); 
        exit(1); 
    }

    for(int i = 0; i < strlen(argv[1]); i++){
        if((argv[1][i] < '0') || (argv[1][i] > '9')){
            fprintf(stderr, "\n<num_fork> deve essere un numero intero."); 
            exit(2); 
        }
    }

    if((nFork = atoi(argv[1])) == 0){
        fprintf(stderr, "\n<num_fork> deve essere un carattere numerico"); 
        exit(2);         
    }

    if(nFork > MAX_PROC){
        fprintf(stderr, "\n<num_fork> deve essere minore o uguale a %d", MAX_PROC); 
        exit(3);  
    }

    int* pid = (int*)malloc(sizeof(int) * nFork); 
    int pipes[MAX_PROC][2]; 

    //Creazione delle pipe
    for(int i = 0; i < MAX_PROC; i++){
        if((pipe(pipes[i])) == -1){
            perror("\nCreazione pipe"); 
            exit(4); 
        }
    }

    for(int i = 0; i < nFork; i++){
        pid[i] = fork(); 
        if(pid[i] == 0){ /*FIGLIO*/
            close(pipes[i][0]); //Chiudo lettura pipe; 
            
            //Creo il messaggio da inviare
            sprintf(buffer, "Ecco qua il figlio al prodifo che scrive... %d\n", getpid()); 
            write(pipes[i][1], buffer, sizeof(buffer) + 1);

            close(pipes[i][1]); //chiudo il canale di scrittura

            exit(0); 
        } else if(pid[i] < 0){
            perror("\nErrore nella creazione del figlio"); 
            exit(3);  
        } else{ /*Padre*/
            close(pipes[i][1]); //chiude il canale di scrittura della pipe
        }
    }

    free(pid); 

    for(int i = 0; i < nFork; i++){
        read(pipes[i][0], buffer, DIM); 
        fprintf(stdout, "\nIl figlio %d dice: %s",i, buffer); 
        close(pipes[i][0]); 
    }

    for(int i = 0; i < nFork; i++){
        wait(&status); 
    } 

    return 0; 
}