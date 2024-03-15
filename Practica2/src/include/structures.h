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
} ;

// create user array
extern int num_users;
extern struct User users[500];