#include <stdio.h>
#include <stdbool.h>

#include "include/operations.h"
#include "include/structures.h"
#include "include/utils.h"

int find_user(int id)
{

    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == id)
        {
            return i;
        }
    }

    return -1;
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

int do_deposit(int account_id, float amount, bool register_error)
{

    char message[100];
    if (find_user(account_id) == -1)
    {
        if (register_error)
        {
            struct ErrorTransactionData error;
            sprintf(error.message, "Error:Usuario con no. de cuenta: %d no encontrado", account_id);
            errors_transaction_data[num_errors_transaction_data] = error;
            num_errors_transaction_data++;
            return -1;
        }
        printf("Error:Usuario con no. de cuenta: %d no encontrado\n", account_id);
        return -1;
    }
    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == account_id)
        {
            users[i].saldo += amount;
            if (!register_error)
            {
                printf("Deposito exitoso. Saldo actual: %f\n", users[i].saldo);
            }
            return 0;
        }
    }
}

int do_money_retirement(int account_id, float amount, bool register_error)
{
    if (find_user(account_id) == -1)
    {
        if (register_error)
        {
            struct ErrorTransactionData error;
            sprintf(error.message, "Error:Usuario con no. de cuenta: %d no encontrado", account_id);
            errors_transaction_data[num_errors_transaction_data] = error;   
            num_errors_transaction_data++;
            return -1;
        }
        printf("Error:Usuario con no. de cuenta: %d no encontrado\n", account_id);
        return -1;
    }

    for (int i = 0; i < num_users; i++)
    {
        if (users[i].id == account_id)
        {
            if (users[i].saldo < amount)
            {
                if (register_error)
                {
                    struct ErrorTransactionData error;
                    sprintf(error.message, "Error: Saldo insuficiente para retiro %f < %f", users[i].saldo, amount);
                    errors_transaction_data[num_errors_transaction_data] = error;
                    num_errors_transaction_data++;
                    return -1;;
                }
                printf("Saldo insuficiente para retiro %f < %f\n", users[i].saldo, amount);
                return -1;
            }
            users[i].saldo -= amount;
            if (!register_error)
            {
                printf("Retiro exitoso. Saldo actual: %f\n", users[i].saldo);
            }
            return 0;
        }
    }
}

int do_transaction(int source_account_id, int destination_account_id, float amount, bool register_error)
{


    int source_account_index = find_user(source_account_id);
    int destination_account_index = find_user(destination_account_id);

    if (source_account_id == -1)
    {
        if (register_error)
        {
            struct ErrorTransactionData error;
            sprintf(error.message, "Error:Usuario con no. de cuenta: %d no encontrado", source_account_id);
            errors_transaction_data[num_errors_transaction_data] = error;
            num_errors_transaction_data++;
            return -1;
        }
        printf("Error:Usuario con no. de cuenta: %d no encontrado\n", source_account_id);
        return -1;
    }
    if (destination_account_index == -1)
    {
        if (register_error)
        {
            struct ErrorTransactionData error;
            sprintf(error.message, "Error:Usuario con no. de cuenta: %d no encontrado", destination_account_id);
            errors_transaction_data[num_errors_transaction_data] = error;
            num_errors_transaction_data++;
            return -1;
        }
        printf("Error:Usuario con no. de cuenta: %d no encontrado\n", destination_account_id);
        return -1;
    }

    if (users[source_account_index].saldo < amount)
    {
        if (register_error)
        {
            struct ErrorTransactionData error;
            sprintf(error.message, "Error: Saldo insuficiente para transferencia %f < %f", users[source_account_index].saldo, amount);
            errors_transaction_data[num_errors_transaction_data] = error;
            num_errors_transaction_data++;
            return -1;
        }
        printf("Error Saldo insuficiente para transferencia %f < %f", users[source_account_index].saldo, amount);
        return -1;
    }

    users[source_account_index].saldo -= amount;
    users[destination_account_index].saldo += amount;
    if (!register_error)
    {
        printf("Transaccion exitosa\n");
    }

    return 0;
}

void get_user_information(int account_id)
{
    if (find_user(account_id) == -1)
    {
        printf("Error:Usuario con no. de cuenta: %d no encontrado\n", account_id);
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

        do_deposit(account_id, amount, false);
    }
    else if (operation_type == 2)
    {
        int account_id = get_int_input_number("Ingrese el id de la cuenta: ");
        float amount = get_float_input_number("Ingrese la cantidad a retirar: ");
        do_money_retirement(account_id, amount, false);
    }
    else if (operation_type == 3)
    {
        int source_account_id = get_int_input_number("Ingrese el id de la cuenta a retirar: ");
        int destination_account_id = get_int_input_number("Ingrese el id de la cuenta a transferir: ");
        float amount = get_float_input_number("Ingrese la cantidad a transferir: ");
        do_transaction(source_account_id, destination_account_id, amount, false);
    }
    else if (operation_type == 4)
    {
        int account_id = get_int_input_number("Ingrese el id de la cuenta: ");
        get_user_information(account_id);
    }
}
