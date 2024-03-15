#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "./include/data_loader.h"
#include "./include/structures.h"

#define MAX_LEN 255


int num_users = 0;
struct User users[500];

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

    // Check if header line is correct
    if (strcmp(header_line, "no_cuenta,nombre,saldo\r\n") != 0 && strcmp(header_line, "no_cuenta,nombre,saldo\n") != 0) {
        printf("Error: Encabezados del archivo no coinciden con: 'no_cuenta,nombre,saldo'\n");
        fclose(file);
        return ;
    }

    // Read user data from three threads
    while (fgets(line, sizeof(line), file)) {

        char *token = strtok(line, ",");
        int column = 0;
        while (token != NULL) {
            // user data
            bool user_is_valid = true;
            int no_cuenta;
            char nombre[50];
            float saldo;
            // ===============
            if (column == 0) {
                //printf("no_cuenta: %s\n", token);
                // Check if no_cuenta is a positive integer
                no_cuenta = atoi(token);
                if (no_cuenta <= 0) {
                    printf("Error: no. de cuenta debe de ser un entero positivo.\n");
                    user_is_valid = false;
                }
            } else if (column == 1) {
                //printf("nombre: %s\n", token);
                strcpy(nombre, token);
            } else if (column == 2) {
                //printf("saldo: %s\n", token);
                // Check if saldo is a positive float value
                saldo = atof(token);
                if (saldo <= 0) {
                    printf("Error: saldo must be a positive float value.\n");
                    // Handle error or exit the loop
                    user_is_valid = false;

                }
            }
            if(user_is_valid == true){
                // save user
                if(num_users < 500){
                    struct User new_user = {no_cuenta};
                    strcpy(new_user.name, nombre); 
                    new_user.saldo = saldo;
                    users[num_users] = new_user;
                    num_users++;
                }
                
            }
            token = strtok(NULL, ",");
            column++;
        }
    }
    

    fclose(file);
    // Print users
    printf("===============Usuarios cargados===============:\n");
    for (int i = 0; i < num_users; i++) {
        printf("no_cuenta: %d, nombre: %s, saldo: %f\n", users[i].id, users[i].name, users[i].saldo);
    }
    
}
