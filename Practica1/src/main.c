#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include "../headers/fileHandler.h"
#define NUMOFCHILDRENPROCESSES 2
// compile command: gcc ./src/main.c ./src/fileHandler.c -o exec

// Variable to track the number of child processes


int pidChildProcess1;
int pidChildProcess2;

void handleCtrlC(int signal) {
    printf("\nCtrl+C received. Terminating processes.\n");

    // Loop to terminate all child processes
    for (int i = 0; i < NUMOFCHILDRENPROCESSES; ++i) {
        wait(NULL); 
    }


     // Open the file for reading
    FILE *file = fopen("syscalls.log", "r");


    // Process the log data
    char *line = NULL;
    size_t len = 0;
    
    // Variables to store process stats
    int read_count[2] = {0};
    int seek_count[2] = {0};
    int write_count[2] = {0};
    int open_count[2] = {0};


    int pids[2] = {pidChildProcess1, pidChildProcess2};

    for (int i = 0; i < 2; i++)
    {
    fseek(file, 0, SEEK_SET);
    while (getline(&line, &len, file) != -1) {
        sscanf(line, "childProcess.bi(%d) ", &pids[i]);

        // Count the occurrences
        char call_type[60];
        sscanf(line, "childProcess.bi(%*d) %s", call_type);

        if (strcmp(call_type, "read") == 0) {
            read_count[i]++;
        } else if (strcmp(call_type, "lseek") == 0) {
            seek_count[i]++;
        }else if (strcmp(call_type, "write") == 0) {
            write_count[i]++;
        }else if (strcmp(call_type, "openat") == 0) {
            open_count[i]++;
        }
    }
    }
    

    // Display the results
    for (int i = 0; i < 2; i++) {
        printf("Process (%d) %d:\n", i + 1, pids[i]);
        printf("  read: %d\n", read_count[i]);
        printf("  seek: %d\n", seek_count[i]);
        printf("  write: %d\n", write_count[i]);
        printf("  open: %d\n", open_count[i]);

        // Add more printf statements for other call types as needed
        printf("\n");
    }

    // Clean up
    free(line);
    fclose(file);

   // fclose(sysCallLogFile);

    exit(0); 
}

int main(){

    int fd [2];
    // Set up the custom signal handler
    signal(SIGINT, handleCtrlC);
    createInitialFiles(); // creates syscall log file and practica1 file
    char *argPtr[2] = {"./practica1.txt", NULL};
    // Command
    char command[100];

    // pointer to syscall log file
    FILE *sysCallLogFile = fopen("./syscalls.log", "w+");

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

        printf("Parent process running with %d child processes.\n", NUMOFCHILDRENPROCESSES);
        printf("Child process 1 PID: %d\n", pidChildProcess1);
        printf("Child process 2 PID: %d\n", pidChildProcess2);

        sprintf(command, "%s %d %d %s", "sudo stap ./src/trace.stp", pidChildProcess1, pidChildProcess2, " > syscalls.log");
        system(command);

        for (int i = 0; i < NUMOFCHILDRENPROCESSES; ++i) {
            wait(NULL);
        }
    }

    fclose(sysCallLogFile);

    return 0;
}