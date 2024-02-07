#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *filename = argv[0];
    
    if(access(filename, F_OK) != -1){
        printf("file exists!\n");
    } else {
        printf("file does not exist\n");
    }

    return 0;
}
