#include <stdio.h>
#include <stdlib.h>

int run_command(const char *command){
    
    //printf("Action:\n");
    
    if(system(command)){
        return 1;
    }

    return 0;
}