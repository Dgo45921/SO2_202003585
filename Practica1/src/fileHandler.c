#include <stdio.h>
#include "../headers/fileHandler.h"

void createInitialFiles()
{
    FILE *sysCallLogFile;
    sysCallLogFile = fopen("./syscalls.log", "w+");

    FILE *practica1File;
    practica1File = fopen("./practica1.txt", "w+");
}