#include <stdio.h>
#include <stdlib.h>

int run_command(const char *command){
    if(system(command)){
        return 1;
    }
    return 0;
}