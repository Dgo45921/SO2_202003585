#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "../headers/fileHandler.h"
#include <stdlib.h>
#include <sys/wait.h>

#include <time.h>
// compile command: gcc ./src/main.c ./src/fileHandler.c -o exec

// Variable to track the number of child processes
int num_children = 2;

// Signal handler function
void handleCtrlC(int signal) {
    printf("\nCtrl+C received. Terminating processes.\n");

    // Loop to terminate all child processes
    for (int i = 0; i < num_children; ++i) {
        wait(NULL); // Wait for each child process to finish
    }

    exit(0); // Terminate the parent process
}


int main()
{
    // Set up the custom signal handler
    signal(SIGINT, handleCtrlC);
    int childProcess1 = fork();

    if (childProcess1 == 0){
        char *argPtr[2] = {"./practica1.txt", NULL};

        int childProcess2 = fork();

        if (childProcess2 == 0){
            // printf("child process 2 logic goes here\n");
            execv("./childProcess.bin", argPtr);
        } else {
            // printf("child process 1 logic goes here\n");
            execv("./childProcess.bin", argPtr);
        }
    }
    else {
        printf("Parent process running with %d child processes.\n", num_children);

    // Parent process waits for Ctrl+C or the child processes to finish
        for (int i = 0; i < num_children; ++i) {
            wait(NULL); // Wait for each child process to finish
        }
    }

    return 0;
}
