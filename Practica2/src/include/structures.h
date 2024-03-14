#include <stdio.h>


struct thread_data {
    FILE* file;
    long start_pos;
    long end_pos;
};

struct user {
    int id;
    char first_name[50];
    char last_name[50];
    char email[50];
};