#include "lib/error_map.h"
#include <string.h>

#define MAX_ERRORS 100

Error error_map[MAX_ERRORS];
int error_count = 0;

// Inicializa o mapa de erros
void initialize_error_map() {

    // Adiciona alguns erros e suas severidades
    add_error_to_map("disk full", 6);
    add_error_to_map("memory leak", 8);
    add_error_to_map("high CPU usage", 4);
    add_error_to_map("system error", 9);

}

// Adiciona um erro ao mapa
void add_error_to_map(const char *log_message, int severity) {
    if (error_count < MAX_ERRORS) {
        error_map[error_count].message = strdup(log_message);
        error_map[error_count].severity = severity;
        error_count++;
    }
}

// Retorna a severidade com base na mensagem de log
int get_error_severity(const char *log_message) {
    for (int i = 0; i < error_count; i++) {
        if (strstr(log_message, error_map[i].message) != NULL) {
            return error_map[i].severity;
        }
    }
    return -1; // Não encontrado
}
