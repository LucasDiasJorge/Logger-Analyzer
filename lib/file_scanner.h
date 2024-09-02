#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct {
    const char *filename;
    const char *target_string;
    const char *command;
} thread_data_t;

void tail_file(const char *filename, const char *target, const char *command);

#endif