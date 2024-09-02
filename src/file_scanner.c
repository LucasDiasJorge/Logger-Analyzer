#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "/home/user/Documentos/Logger-Analyzer/lib/action.h"

#define BUFFER_SIZE 1024

typedef struct {
    const char *filename;
    const char *target_string;
    const char *command;
} thread_data_t;

void tail_file(const char *filename, const char *target_string, const char *command) {
    
    FILE *file;
    char buffer[BUFFER_SIZE];
    long position;

    file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fseek(file, 0, SEEK_END);
    position = ftell(file);

    while (1) {
        fseek(file, position, SEEK_SET);

        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            printf("Read line: %s\n", buffer);

            if (strstr(buffer, target_string) != NULL) {
                printf("Found the target string: %s\n", target_string);
                run_command(command);
            }

            position = ftell(file);
        }

        sleep(5);

        file = freopen(filename, "r", file);
        if (!file) {
            perror("Failed to reopen file");
            return;
        }

        fseek(file, position, SEEK_SET);
    }

    fclose(file);
}
