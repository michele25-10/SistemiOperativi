#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define DIM_COD 6
#define DIM_DATA 9
#define DIM_PATH 250

static volatile sig_atomic_t count = 0; 

//Mi basta anche un handler vuoto, è per far uscire p1 dalla pause()
void handler_sigusr1(int signum){}

void handler_sigint(int signum){
    printf("\nIl numero di iterazione è: %d\n", count); 
    exit(0); 
}

int main(int argc, char**argv){
    int p1p2[2]; int p2p3[2]; 
    int pid1, pid2, pid3;
    int status; 
    char cod[DIM_COD]; char data[DIM_DATA]; char path[DIM_PATH]; 
    int n; 

    if(argc != 2){
        printf("Numero di argomenti erratto\n"); 
        exit(1); 
    }

    if(argv[1][0] == '/'){
        printf("Il percorso non è relativo\n"); 
        exit(2); 
    }

    //Controllo se la dir esiste
    int dir = open(argv[1], __O_DIRECTORY); 
    if(dir < 0){
        printf("\nDirectory non esistente\n"); 
        exit(2); 
    }
    close(dir); 

    signal(SIGINT, handler_sigint); 

    //Ciclo di richieste dati in input all'utente
    while(1){
        printf("\nCodice: "); 
        scanf("%s", cod); 
        printf("\nData: "); 
        scanf("%s", data); 
        printf("\nNumero risulati da vedere: "); 
        scanf("%d", &n); 

        printf("\n"); 

        //Compongo il percorso
        sprintf(path, "%s/%s.txt", argv[1], data); 

        int fd = open(path, O_RDONLY); 
        if(fd < 0){
            printf("File non esistente"); 
            continue; 
        }
        close(fd);


        if(n <= 0 || n > 999) {
            printf("\nNumero non valido");
            continue;  
        } 

        //Creazione prima pipe
        if(pipe(p1p2) < 0 ){
            printf("\nErrore pipe"); 
            exit(4); 
        }

        pid1 = fork(); 
        if(pid1 < 0){
            printf("\nErrore p1\n"); 
            exit(5); 
        }
        if(pid1 == 0){
            signal(SIGINT, SIG_DFL); //solo per p1 p2 p3
            signal(SIGUSR1, handler_sigusr1); //solo per p1

            pause(); 

            close(p1p2[0]); 

            close(1); 
            dup(p1p2[1]); 
            close(p1p2[1]); 

            execlp("grep", "grep", cod, path, (char*)0); 
            printf("\nErrore grep\n"); 

            exit(6); 
        }

        //Se chiudo p1p2[1] qui non ho bisogno di richiuderlo nel padre
        close(p1p2[1]); 

        //genero la seconda pipe
        if(pipe(p2p3) < 0){
            printf("\nErrore pipe\n"); 
            exit(7); 
        }

        pid2 = fork(); 
        if(pid2 < 0){
            printf("\nErrore p2\n"); 
            exit(8); 
        }
        if(pid2 == 0){
            signal(SIGINT, SIG_DFL); 

            close(0); 
            dup(p1p2[0]); 
            close(p1p2[0]); 

            close(p2p3[0]); 
            
            close(1); 
            dup(p2p3[1]);
            close(p2p3[0]); 

            //riordina in modo numerico la seconda key splitate in base alla virgola
            execlp("sort", "sort", "-k", "2", "-n", "-t", ",",  (char*)0); 
            printf("\nErrore nella sort\n"); 
            exit(9); 
        }

        close(p1p2[0]); 
        close(p2p3[1]); 

        pid3 = fork(); 
        if(pid3 < 0){
            printf("\nErrore p3\n"); 
            exit(10); 
        }
        if(pid3 == 0){
            signal(SIGINT, SIG_DFL); 

            close(0); 
            dup(p2p3[0]); 
            close(p2p3[0]); 

            char n_str[4]; 
            sprintf(n_str, "%d", n); 

            printf("%s", n_str); 

            execlp("tail", "tail", "-n", n_str, (char*)0); 
            printf("\nErrore nella head\n"); 
            exit(11);  
        }

        close(p2p3[0]);

        sleep(1); 

        //Invio segnale a p1
        kill(pid1, SIGUSR1); 

        //Attendo i processi figli
        wait(&status); 
        wait(&status); 
        wait(&status); 

        count++; 
    }
}
