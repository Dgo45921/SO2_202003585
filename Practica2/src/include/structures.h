#include <stdio.h>

struct User {
    int id;
    char name[50];
    float saldo;
};

struct ThreadData{
    FILE* file;
    long start_pointer;
    int line_count;
    int users_added;
} ;

struct ThreadTransactionData{
    FILE* file;
    long start_pointer;
    int line_count;
    int retirements_added;
    int deposits_added;
    int transfers_added;
} ;

struct ErrorData{
    char message[100];
} ;

struct ErrorTransactionData{
    char message[100];
} ;

// create user array
extern int num_users;
extern struct User users[500];


// create user load error array
extern int num_errors_user_load;
extern struct ErrorData errors_user_load[500];


// create user load error array
extern int num_errors_transaction_data;
extern struct ErrorTransactionData errors_transaction_data[500];
