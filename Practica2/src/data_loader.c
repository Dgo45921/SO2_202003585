#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "./include/data_loader.h"
#define MAX_LEN 1024

FILE* open_file(char* file_path){
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }
    return file;
}

long get_file_size(FILE* file){
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return file_size;
}

char *ask_file_path() {
    static char file_path[256];
    printf("Ingrese la ruta del archivo de usuarios .csv: ");
    scanf("%255s", file_path);
    if (strstr(file_path, ".csv") == NULL) {
        printf("Error: La ruta del archivo debe terminar con .csv.\n");
        return NULL;
    }
    return file_path;
}


void load_users(){
    char *file_path = ask_file_path();
    if (file_path == NULL) {
        return;
    }

    FILE* file = open_file(file_path);
    long file_size = get_file_size(file);

    char line[MAX_LEN];
    char* header_line = fgets(line, sizeof(line), file); // get header line

    if (strcmp(header_line, "no_cuenta,nombre,saldo\r\n") != 0 && strcmp(header_line, "no_cuenta,nombre,saldo\n") != 0) {
        printf("Error: Encabezados del archivo no coinciden con: 'no_cuenta,nombre,saldo'\n");
        fclose(file);
        return ;
    }

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        while (token != NULL) {
            printf("%s ", token);
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
    
}

