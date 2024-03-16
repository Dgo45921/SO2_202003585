
#include "include/utils.h"


float transform_float_2d(float number) {
    return ((int)(number * 100)) / 100.0;
}


void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
