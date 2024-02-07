#include <stdio.h>
#include <unistd.h>
#include "../headers/fileHandler.h"
// compile command: gcc ./src/main.c ./src/fileHandler.c -o exec

int main()
{
    createInitialFiles();
    int childProcess1 = fork();

    if (childProcess1 == 0){
        char *argPtr[2] = {"./practica1.txt", NULL};
        int childProcess2 = fork();
        if (childProcess2 == 0){
            printf("child process 2 logic goes here\n");
            execv("./childProcess.bin", argPtr);
        } else {
            printf("child process 1 logic goes here\n");
            execv("./childProcess.bin", argPtr);
        }
    }
    else {
        printf("Parent process logic goes here\n");
    }

    return 0;
}
