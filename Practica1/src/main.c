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

// pointer to syscall log file
FILE *sysCallLogFile;

void handleCtrlC(int signal) {
    printf("\nCtrl+C received. Terminating processes.\n");

    // Loop to terminate all child processes
    for (int i = 0; i < num_children; ++i) {
        wait(NULL); 
    }

   // fclose(sysCallLogFile);

    exit(0); 
}

int main()
{
    // Set up the custom signal handler
    signal(SIGINT, handleCtrlC);
    createInitialFiles(); // creates syscall log file and practica1 file
    char *argPtr[2] = {"./practica1.txt", NULL};
    // Command
    char command[100];
    int pidChildProcess1;
    int pidChildProcess2;

    sysCallLogFile = fopen("./syscalls.log", "w+");

    int childProcess1 = fork();

    if (childProcess1 == 0) {
        // Child process 1 logic goes here  
        execv("./childProcess.bin", argPtr);
        
    } else {

        int childProcess2 = fork();

        if (childProcess2 == 0) {
            // Child process 2 logic goes here
            execv("./childProcess.bin", argPtr);
        }
        pidChildProcess1 = childProcess1;
        pidChildProcess2 = childProcess2;

        printf("Parent process running with %d child processes.\n", num_children);
        printf("Child process 1 PID: %d\n", pidChildProcess1);
        printf("Child process 2 PID: %d\n", pidChildProcess2);

        // Parent process waits for Ctrl+C or the child processes to finish
        sprintf(command, "%s %d %d %s", "sudo stap ./src/trace.stp", pidChildProcess1, pidChildProcess2, " > syscalls.log");
        //printf("%s", command);
        system(command);

        for (int i = 0; i < num_children; ++i) {
            wait(NULL);
        }
    }

    fclose(sysCallLogFile);

    return 0;
}