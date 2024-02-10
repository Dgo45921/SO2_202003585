#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


void writeOperation(FILE *file){
    const char characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    const int stringLength = 8;

    char randomString[stringLength + 1]; // +1 for the null terminator

    for (int i = 0; i < stringLength; ++i) {
        int index = rand() % (sizeof(characters) - 1);
        randomString[i] = characters[index];
    }
    // null char
    randomString[stringLength] = '\0';

    fprintf(file, "%s", randomString);
    
}

void readEightChars(FILE *file){
    char buffer [9];
    fread(buffer, sizeof(char), 8, file);
}

void resetFilePointer(FILE *file){
    fseek(file, 0, SEEK_SET);
}

int getRandomOperationIndex(){
    return rand() % 3;
}



int main(int argc, char *argv[])
{
    // Seed for random generation based on the current time
    srand((unsigned int)time(NULL));
    const char *filename = argv[0];
    
    if(access(filename, F_OK) != -1){

        while (1){
            int randomSleepSeconds = rand() % 3 + 1;
            sleep(randomSleepSeconds);
            int randomOperation = getRandomOperationIndex();
            FILE *practicaFile = fopen(filename,"w");
            if (randomOperation == 0){
                writeOperation(practicaFile);
            }
            else if (randomOperation == 1){
                // read 8 chars
                readEightChars(practicaFile);
            } else{
                // lseek 
                resetFilePointer(practicaFile);
            }
        }
    } 
    else return -1;
    

    return 0;
}


