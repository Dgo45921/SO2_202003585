#include <stdio.h>
#include "./include/menus.h"
#include "./include/data_loader.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_principal_menu(){
    int option;

    do {
        printf("============Menu===========\n");
        printf("1. Carga masiva\n");
        printf("2. Operaciones individuales\n");
        printf("3. Reportes\n");
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
                printf("Reportes\n");
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
                printf("Reportes\n");
                break;
            default:
                printf("Opción no válida. Por favor, ingrese una opción válida.\n");
                break;
        }
    } while (option != 3);
}