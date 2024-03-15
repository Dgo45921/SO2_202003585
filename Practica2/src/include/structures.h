#include <stdio.h>


struct thread_data {
    FILE* file;
    long start_pos;
    long end_pos;
};

struct User {
    int id;
    char name[50];
    float saldo;
};

// create user array
extern int num_users;
extern struct User users[500];