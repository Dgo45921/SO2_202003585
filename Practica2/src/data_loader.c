#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "./include/data_loader.h"
#include "./include/structures.h"
#include "./include/utils.h"
#include "./include/operations.h"

#define MAX_LEN 255
#define NUM_THREADS 3
#define NUM_THREADS_TRANSACTIONS 4

pthread_mutex_t mutex;

int num_errors_user_load = 0;
struct ErrorData errors_user_load[500];

int num_errors_transaction_data = 0;
struct ErrorTransactionData errors_transaction_data[500];

bool hasLetters(const char *str)
{
    while (*str)
    {
        if (isalpha(*str))
        {
            return true;
        }
        str++;
    }
    return false;
}

bool isFloat(const char *str)
{
    char *endptr;
    strtof(str, &endptr);

    // Check if the entire string was consumed and it doesn't contain any letters
    return *endptr == '\n' && !hasLetters(str) || *endptr == '\0' && !hasLetters(str);
}

FILE *open_file(char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }
    return file;
}

long get_file_size(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return file_size;
}

char *ask_file_path()
{
    static char file_path[256];
    printf("Ingrese la ruta del archivo de usuarios .csv: ");
    scanf("%255s", file_path);
    if (strstr(file_path, ".csv") == NULL)
    {
        printf("Error: La ruta del archivo debe terminar con .csv.\n");
        return NULL;
    }
    return file_path;
}

int count_lines(FILE *file)
{
    int count = 0;
    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        count++;
    }
    rewind(file); // Reset file pointer to the beginning
    return count;
}

void *thread_read(void *thread_data)
{
    struct ThreadData *thread_file_pointer = (struct ThreadData *)thread_data;
    FILE *file = thread_file_pointer->file;
    long start_pointer = thread_file_pointer->start_pointer;
    int line_count = thread_file_pointer->line_count;
    fseek(file, start_pointer, SEEK_SET);
    char line[MAX_LEN];
    int counter = 0;
    pthread_mutex_lock(&mutex);

    while (fgets(line, sizeof(line), file) && counter < line_count)
    {

        // user data
        int no_cuenta = -1;
        char nombre[50] = "";
        float saldo = -1;
        // ===============
        bool error_found = false;

        char *token = strtok(line, ",");
        int column = 0;
        while (token != NULL)
        {

            if (column == 0)
            {
                no_cuenta = atoi(token);
                if (no_cuenta <= 0 && token[0] != '0')
                {
                    if (num_errors_user_load < 500)
                    {
                        struct ErrorData new_error;
                        sprintf(new_error.message, "Error: codigo debe de ser un entero positivo %s", token);
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    error_found = true;
                    break;
                }
            }
            else if (column == 1)
            {
                // compare that the name is not empty string
                if (strcmp(token, "") == 0)
                {

                    if (num_errors_user_load < 500)
                    {
                        struct ErrorData new_error = {"Error: nombre no puede ser una cadena vacía.\n"};
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    error_found = true;
                    break;
                }
                strcpy(nombre, token);
            }
            else if (column == 2)
            {
                saldo = atof(token);

                if (!isFloat(token) || saldo < 0)
                {
                    if (num_errors_user_load < 500)
                    {
                        struct ErrorData new_error;
                        sprintf(new_error.message, "Error: saldo debe de ser un positivo real %s", token);
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    error_found = true;
                    break;
                }
            }

            token = strtok(NULL, ",");
            column++;
        }

        if (!error_found)
        {
            for (int i = 0; i < num_users; i++)
            {
                if (users[i].id == no_cuenta)
                {

                    if (num_errors_user_load < 500)
                    {
                        struct ErrorData new_error;
                        sprintf(new_error.message, "Error: El usuario con el no. de cuenta %d ya existe.\n", no_cuenta);
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    break;
                }
            }

            if (num_users < 500)
            {
                struct User new_user = {no_cuenta};
                strcpy(new_user.name, nombre);
                new_user.saldo = saldo;
                users[num_users] = new_user;
                num_users++;
                thread_file_pointer->users_added = thread_file_pointer->users_added + 1;
            }
        }
        counter++;
    }

    pthread_mutex_unlock(&mutex);
    fclose(file);
    pthread_exit(NULL);
}

void *thread_read_transaction(void *thread_data)
{
    struct ThreadTransactionData *thread_file_pointer = (struct ThreadTransactionData *)thread_data;
    FILE *file = thread_file_pointer->file;
    long start_pointer = thread_file_pointer->start_pointer;
    int line_count = thread_file_pointer->line_count;
    fseek(file, start_pointer, SEEK_SET);
    char line[MAX_LEN];
    int counter = 0;
    pthread_mutex_lock(&mutex);

    while (fgets(line, sizeof(line), file) && counter < line_count)
    {

        int operation = -1;
        int source_account_id = -1;
        int destination_account_id = -1;
        float amount = -1;
        bool error_found = false;

        char *token = strtok(line, ",");
        int column = 0;
        while (token != NULL)
        {

            // token[strcspn(token, "\n")] = '\0';

            if (column == 0)
            {
                operation = atoi(token);
                if (operation != 1 || operation != 2 || operation != 3)
                {
                    if (num_errors_user_load < 500)
                    {
                        struct ErrorTransactionData new_error;
                        sprintf(new_error.message, "Error: operacion debe ser 1 o 2 o 3 %s", token);
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    error_found = true;
                    break;
                }
            }
            else if (column == 1)
            {
                source_account_id = atoi(token);
                if (source_account_id <= 0)
                {
                    if (num_errors_user_load < 500)
                    {
                        struct ErrorTransactionData new_error;
                        sprintf(new_error.message, "Error: numero de cuenta origen debe de ser un entero positivo %s", token);
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    error_found = true;
                    break;
                }
            }
            else if (column == 2)
            {
                source_account_id = atoi(token);
                if (source_account_id <= 0)
                {
                    if (num_errors_user_load < 500)
                    {
                        struct ErrorTransactionData new_error;
                        sprintf(new_error.message, "Error: numero de cuenta destino debe de ser un entero positivo %s", token);
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    error_found = true;
                    break;
                }
            }
            else if (column == 3)
            {
                amount = atof(token);

                if (!isFloat(token) || amount < 0)
                {
                    if (num_errors_user_load < 500)
                    {
                        struct ErrorTransactionData new_error;
                        sprintf(new_error.message, "Error: saldo debe de ser un positivo real %s", token);
                        strcpy(errors_user_load[num_errors_user_load].message, new_error.message);
                        num_errors_user_load++;
                    }
                    error_found = true;
                    break;
                }
            }

            token = strtok(NULL, ",");
            column++;
        }
        counter++;
        if (!error_found)
        {

            if (operation == 1)
            {
                if (do_deposit(source_account_id, amount, true) == 0)
                {
                    thread_file_pointer->deposits_added = thread_file_pointer->deposits_added + 1;
                }
            }
            else if (operation == 2)
            {
                if (do_money_retirement(source_account_id, amount, true) == 0)
                {
                    thread_file_pointer->retirements_added = thread_file_pointer->retirements_added + 1;
                }
            }
            else if (operation == 3)
            {
                if (do_transaction(source_account_id, destination_account_id, amount, true)){
                    thread_file_pointer->transfers_added = thread_file_pointer->transfers_added + 1;
                }
            }
        }
    }

    pthread_mutex_unlock(&mutex);
    fclose(file);
    pthread_exit(NULL);
}

void create_user_load_report(struct ThreadData data_threads[])
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Create file name with the specified format
    char filename[50];
    sprintf(filename, "carga_%d_%02d_%02d-%02d_%02d_%02d.log",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    // Open the file for writing
    FILE *file = fopen(filename, "w+");
    if (file == NULL)
    {
        printf("Error creando archivo de reporte carga masiva usuarios!\n");
        return;
    }

    fprintf(file, "=====Reporte de carga masiva de usuarios=====\n");

    fprintf(file, "Fecha: %d_%02d_%02d %02d_%02d_%02d\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    fprintf(file, "====Usuarios cargados====\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        fprintf(file, "     Hilo #%d: %d\n", i + 1, data_threads[i].users_added);
    }

    fprintf(file, "Total: %d\n", num_users);

    fprintf(file, "=====Errores======:\n");

    for (int i = 0; i < num_errors_user_load; i++)
    {
        fprintf(file, "     %d. %s", i + 1, errors_user_load[i].message);
    }

    fclose(file);
}

void create_transaction_load_report(struct ThreadTransactionData data_threads[])
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Create file name with the specified format
    char filename[50];
    sprintf(filename, "operaciones_%d_%02d_%02d-%02d_%02d_%02d.log",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    // Open the file for writing
    FILE *file = fopen(filename, "w+");
    if (file == NULL)
    {
        printf("Error creando archivo de reporte de carga masiva de operaciones!\n");
        return;
    }

    fprintf(file, "=====Resumen de operaciones=====\n");

    fprintf(file, "Fecha: %d-%02d-%02d %02d-%02d-%02d\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    fprintf(file, "====Operaciones realizadas====\n");

    int total_deposits = 0;
    int total_retirements = 0;
    int total_transfers = 0;
    


    for (int i = 0; i < NUM_THREADS_TRANSACTIONS; i++)
    {
        total_deposits += data_threads[i].deposits_added;
        total_retirements += data_threads[i].retirements_added;
        total_transfers += data_threads[i].transfers_added;
    }

    fprintf(file, "     Depositos: %d\n", total_deposits);
    fprintf(file, "     Retiros: %d\n", total_retirements);
    fprintf(file, "     Transferencias: %d\n", total_transfers);
    fprintf(file, "Total: %d\n", total_deposits + total_retirements + total_transfers);

    fprintf(file, "====Operaciones por hilo====\n");
    for (int i = 0; i < NUM_THREADS_TRANSACTIONS; i++)
    {
        fprintf(file, "     Hilo #%d: %d\n", i+1 ,data_threads[i].deposits_added + data_threads[i].retirements_added + data_threads[i].transfers_added );
    }

    fprintf(file, "=====Errores======:\n");

    for (int i = 0; i < num_errors_transaction_data; i++)
    {
        fprintf(file, "     %d. %s\n", i + 1, errors_transaction_data[i].message);
    }

    fclose(file);
}
void separate_file(FILE *file, char *file_path)
{

    char line[MAX_LEN];

    int total_lines = count_lines(file);
    fgets(line, sizeof(line), file);

    int linesPerWorker = total_lines / NUM_THREADS;
    int remaininglines = total_lines % NUM_THREADS;

    int linesForWorker1 = linesPerWorker;
    int linesForWorker2 = linesPerWorker;
    int linesForWorker3 = linesPerWorker + remaininglines;

    long fp_thread_1 = ftell(file);

    for (int i = 0; i < linesForWorker1; i++)
    {
        char *test = fgets(line, sizeof(line), file);
    }

    long fp_thread_2 = ftell(file);

    for (int i = 0; i < linesForWorker2; i++)
    {
        char *test = fgets(line, sizeof(line), file);
    }

    long fp_thread_3 = ftell(file);

    // Create threads
    pthread_t threads[NUM_THREADS];

    struct ThreadData data_threads[3];
    long pointers[3] = {fp_thread_1, fp_thread_2, fp_thread_3};

    for (int i = 0; i < NUM_THREADS; i++)
    {
        FILE *file = open_file(file_path);
        data_threads[i].file = file;
        data_threads[i].start_pointer = pointers[i];
        data_threads[i].users_added = 0;
    }

    data_threads[0].line_count = linesForWorker1;
    data_threads[1].line_count = linesForWorker2;
    data_threads[2].line_count = linesForWorker3;

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        int rc = pthread_create(&threads[i], NULL, thread_read, (void *)&data_threads[i]);
        if (rc)
        {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return;
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("*******Usuarios cargados*******\n");
    create_user_load_report(data_threads);
}

void separate_transaction_file(FILE *file, char *file_path)
{

    char line[MAX_LEN];

    int total_lines = count_lines(file);
    fgets(line, sizeof(line), file);

    int linesPerWorker = total_lines / NUM_THREADS_TRANSACTIONS;
    int remaininglines = total_lines % NUM_THREADS_TRANSACTIONS;

    int linesForWorker1 = linesPerWorker;
    int linesForWorker2 = linesPerWorker;
    int linesForWorker3 = linesPerWorker;
    int linesForWorker4 = linesPerWorker + remaininglines;

    long fp_thread_1 = ftell(file);

    for (int i = 0; i < linesForWorker1; i++)
    {
        char *test = fgets(line, sizeof(line), file);
    }

    long fp_thread_2 = ftell(file);

    for (int i = 0; i < linesForWorker2; i++)
    {
        char *test = fgets(line, sizeof(line), file);
    }

    long fp_thread_3 = ftell(file);

    for (int i = 0; i < linesForWorker3; i++)
    {
        char *test = fgets(line, sizeof(line), file);
    }

    long fp_thread_4 = ftell(file);

    // Create threads
    pthread_t threads[NUM_THREADS_TRANSACTIONS];

    struct ThreadTransactionData data_threads[4];
    long pointers[4] = {fp_thread_1, fp_thread_2, fp_thread_3, fp_thread_4};

    for (int i = 0; i < NUM_THREADS_TRANSACTIONS; i++)
    {
        FILE *file = open_file(file_path);
        data_threads[i].file = file;
        data_threads[i].start_pointer = pointers[i];
    }

    data_threads[0].line_count = linesForWorker1;
    data_threads[1].line_count = linesForWorker2;
    data_threads[2].line_count = linesForWorker3;
    data_threads[3].line_count = linesForWorker4;

    for (int i = 0; i < NUM_THREADS_TRANSACTIONS; i++)
    {
        data_threads[i].retirements_added = 0;
        data_threads[i].deposits_added = 0;
        data_threads[i].transfers_added = 0;
    }

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS_TRANSACTIONS; i++)
    {
        int rc = pthread_create(&threads[i], NULL, thread_read_transaction, (void *)&data_threads[i]);
        if (rc)
        {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return;
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS_TRANSACTIONS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("*******Operaciones efectuadas*******\n");
    create_transaction_load_report(data_threads);
}

void load_users()
{
    char *file_path = ask_file_path();
    if (file_path == NULL)
    {
        return;
    }

    FILE *file = open_file(file_path);

    char line[MAX_LEN];
    char *header_line = fgets(line, sizeof(line), file); // get header line

    // Check if header line is correct
    if (strcmp(header_line, "no_cuenta,nombre,saldo\r\n") != 0 && strcmp(header_line, "no_cuenta,nombre,saldo\n") != 0)
    {
        printf("Error: Encabezados del archivo no coinciden con: 'no_cuenta,nombre,saldo'\n");
        fclose(file);
        return;
    }
    separate_file(file, file_path);

    fclose(file);
    // Print users
    // printf("===============Usuarios cargados===============:\n");
    // for (int i = 0; i < num_users; i++) {
    //     printf("no_cuenta: %d, nombre: %s, saldo: %f\n", users[i].id, users[i].name, users[i].saldo);
    // }
}

void do_transactions()
{
    char *file_path = ask_file_path();
    if (file_path == NULL)
    {
        return;
    }

    FILE *file = open_file(file_path);

    char line[MAX_LEN];
    char *header_line = fgets(line, sizeof(line), file); // get header line

    // Check if header line is correct
    if (strcmp(header_line, "operacion,cuenta1,cuenta2,monto\r\n") != 0 && strcmp(header_line, "operacion,cuenta1,cuenta2,monto\n") != 0)
    {
        printf("Error: Encabezados del archivo no coinciden con: 'operacion,cuenta1,cuenta2,monto'\n");
        fclose(file);
        return;
    }
    separate_transaction_file(file, file_path);

    fclose(file);
}