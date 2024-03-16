#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/operations.h"

bool find_user(int id){

    return false;
}


void* get_input_number(const char *prompt) {
    char input[100];
    int number;

    while (true) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &number) == 1) {
            return number;
        } else {
            printf("Numero no valido.\n");
            return NULL;
        }
    }
}

void do_deposit(){
    printf("Deposito\n");
}


void do_money_retirement(){
    printf("Retiro\n");

}

void do_transaction(){
    printf("Transaccion\n");
}

void get_user_information(){
    printf("Consultar cuenta\n");
}

void do_operation(int operation_type){

    if(operation_type == 1){
        do_deposit();
    }else if(operation_type == 2){
        do_money_retirement();
    }else if(operation_type == 3){
        do_transaction();
    }else if(operation_type == 4){
        get_user_information();
    }

}

