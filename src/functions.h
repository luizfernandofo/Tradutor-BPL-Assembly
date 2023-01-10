#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>

typedef struct
{
    uint8_t qty_params;
    char param_type[3];

    uint8_t qty_local_var;
    char local_var_type[5];

}Function;


#endif