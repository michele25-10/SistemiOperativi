#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
    if (argc != 2){
        printf("copy_input <nome_file>");
        return -1;
    }

    int fd = open(argv[1], O_CREAT|O_WRONLY, 0644); 
    if(fd < 0){
        fprintf(stderr, "Errore apretura file %s", argv[1]); 
    }

    char buff[1024]; 
    printf("\nCopia le stringhe finchè la stirnga inserita non è uguale a fine\n"); 
    fgets(buff, 1024, stdin);      
    
    while(strcmp(buff, "fine\n") != 0){
        write(fd, buff, strlen(buff)); 
        printf("\nDigita la stringa: "); 
        fgets(buff, 1024, stdin); 
    }

    close(fd); 

    return 0;
}