#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/operations.h"
#include "include/structures.h"
#include "include/utils.h"


bool find_user(int id)
{

    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == id)
        {
            return true;
        }
    }

    return false;
}

int get_int_input_number(const char *prompt)
{
    char input[100];
    int number;

    while (true)
    {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &number) == 1 && number >= 0)
        {
            return number;
        }
        else
        {
            printf("Numero no valido.\n");
        }
    }
}


float get_float_input_number(const char *prompt)
{
    char input[100];
    float number;

    while (true)
    {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%f", &number) == 1 && number >= 0)
        {
            return number;
        }
        else
        {
            printf("Numero no valido.\n");
        }
    }
}



void do_deposit(int account_id, float amount)
{
    if (find_user(account_id) == false)
    {
        printf("Usuario con no. de cuenta: %d no encontrado\n", account_id);
        return;
    }
    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == account_id)
        {
            users[i].saldo += amount;
            printf("Deposito exitoso. Saldo actual: %f\n", users[i].saldo);
            return;
        }
    }
}

void do_money_retirement(int account_id, float amount)
{
    if (find_user(account_id) == false)
    {
        printf("Usuario con no. de cuenta: %d no encontrado\n", account_id);
        return;
    }

    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == account_id)
        {
            if (users[i].saldo < amount)
            {
                printf("Saldo insuficiente\n");
                return;
            }
            users[i].saldo -= amount;
            printf("Retiro exitoso. Saldo actual: %f\n", users[i].saldo);
            return;
        }
    }
}

void do_transaction(int source_account_id, int destination_account_id, float amount)
{
    printf("Transaccion\n");
    if (find_user(source_account_id) == false)
    {
        printf("Usuario con no. de cuenta: %d no encontrado\n", source_account_id);
        return;
    }
    if (find_user(destination_account_id) == false)
    {
        printf("Usuario con no. de cuenta: %d no encontrado\n", destination_account_id);
        return;
    }

    int source_index = -1;
    int destination_index = -1;
    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == source_account_id)
        {
            source_index = i;
        }
        if (users[i].id == destination_account_id)
        {
            destination_index = i;
        }


    }

    if (source_index != -1 && destination_index != -1)
    {
        printf("No se hallaron usuarios con los numeros de cuenta dados\n");
        return;
    }

    if (users[source_index].saldo < amount)
    {
        printf("Saldo insuficiente\n");
        return;
    }

    users[source_index].saldo -= amount;
    users[destination_index].saldo += amount;
    printf("Transaccion exitosa\n");
}

void get_user_information(int account_id)
{
    if (find_user(account_id) == false)
    {
        printf("Usuario con no. de cuenta: %d no encontrado\n", account_id);
        return;
    }

    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == account_id)
        {
            printf("Nombre: %s\n", users[i].name);
            printf("Saldo: %f\n", users[i].saldo);
            return;
        }
    }
}

void do_individual_operation(int operation_type)
{

    clear_input_buffer();


    if (operation_type == 1)
    {
        int account_id = get_int_input_number("Ingrese el id de la cuenta: ");
        float amount = get_float_input_number("Ingrese la cantidad a depositar: ");

        do_deposit(account_id, amount);
    }
    else if (operation_type == 2)
    {
        int account_id = get_int_input_number("Ingrese el id de la cuenta: ");
        float amount = get_float_input_number("Ingrese la cantidad a retirar: ");
        do_money_retirement(account_id, amount);
    }
    else if (operation_type == 3)
    {
        int source_account_id = get_int_input_number("Ingrese el id de la cuenta a retirar: ");
        int destination_account_id = get_int_input_number("Ingrese el id de la cuenta a transferir: ");
        float amount = get_float_input_number("Ingrese la cantidad a transferir: ");
        do_transaction(source_account_id, destination_account_id, amount);
    }
    else if (operation_type == 4)
    {
        int account_id = get_int_input_number("Ingrese el id de la cuenta: ");
        get_user_information(account_id);
    }
}
