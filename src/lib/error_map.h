#ifndef ERROR_MAP_H
#define ERROR_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estrutura de um erro no mapa
typedef struct Error {
    char *message;
    int severity;
} Error;

// Funções para lidar com o mapa de erros
void initialize_error_map();
int get_error_severity(const char *log_message);
void add_error_to_map(const char *log_message, int severity);

#endif
