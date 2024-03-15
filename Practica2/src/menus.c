#include <stdio.h>
#include <pthread.h>
#include "./include/menus.h"
#include "./include/data_loader.h"
#include "./include/structures.h"


void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void acounts_report(){
    char filename[50];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(filename, "estado_cuentas_%d_%02d_%02d-%02d_%02d_%02d.csv", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    // Open the file for writing
    FILE *file = fopen(filename, "w+");
    if (file == NULL) {
        printf("Error creando archivo de reporte estado de cuentas!\n");
        return ;
    }

    fprintf(file, "no_cuenta,nombre,saldo\n");
    for (int i = 0; i < num_users; i++){
        fprintf(file, "%d,%s,%f\n", users[i].id, users[i].name, users[i].saldo);
    }
    
    fclose(file);

}

void print_principal_menu(){
    int option;

    do {
        printf("============Menu===========\n");
        printf("1. Carga masiva\n");
        printf("2. Operaciones individuales\n");
        printf("3. Reporte estado de cuenta\n");
        printf("4. Salir\n");
        printf("Ingrese su opción: ");
        
        if (scanf("%d", &option) != 1) {
            clear_input_buffer();
            printf("Opción no válida. Por favor, ingrese una opción válida.\n");
            continue;
        }
        

        switch (option) {
            case 1:
                print_load_menu();
                break;
            case 2:
                printf("Operaciones individuales\n");
                break;
            case 3:
                acounts_report();
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Por favor, ingrese una opción válida.\n");
                break;
        }
    } while (option != 4);
}


void print_load_menu(){
    int option;

    do {
        printf("=========Carga masiva========\n");
        printf("1. Cargar usuarios\n");
        printf("2. Cargar operaciones\n");
        printf("3. Retornar\n");
        printf("Ingrese su opción: ");
        
        // Read input
        if (scanf("%d", &option) != 1) {
            clear_input_buffer(); // Clear input buffer
            printf("Opción no válida. Por favor, ingrese una opción válida.\n");
            continue;
        }
        

        // Process the selected option
        switch (option) {
            case 1:
                printf("1. Cargar usuarios\n");
                load_users();
                break;
            case 2:
                printf("2. Cargar operaciones\n");
                break;
            case 3:
                printf("3. Retornar\n");
                break;
            default:
                printf("Opción no válida. Por favor, ingrese una opción válida.\n");
                break;
        }
    } while (option != 3);
}