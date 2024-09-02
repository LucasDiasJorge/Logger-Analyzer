#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void tail_file(const char *filename, const char *target, const char *command);

#endif