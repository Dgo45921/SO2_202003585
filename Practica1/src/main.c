#include <stdio.h>
#include <unistd.h>
#include "../headers/fileHandler.h"
// compile command: gcc ./src/main.c ./src/fileHandler.c -o exec

int main()
{
    createInitialFiles(0);
    int childProcess1 = fork();

    if(childProcess1 == 0){
        int childProcess2 = fork();
        if(childProcess2 == 0){
            printf("child process 2 logic goes here\n");
        } else {
            printf("child process 1 logic goes here\n");

        }

    } else{
        printf("Parent process logic goes here\n");
    }


    return 0;
}
