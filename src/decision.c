#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lib/decision_tree.h"

#define ANSI_RESET_ALL          "\x1b[0m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_GREEN        "\x1b[32m"

// Definição de códigos de ação
#define ACTION_RESTART 1
#define ACTION_SHUTDOWN 2
#define ACTION_ALERT 3
#define ACTION_LOG 4
#define ACTION_REPAIR 5
#define ACTION_SLOWDOWN 6

// Função para limpar a mensagem de log, removendo caracteres não imprimíveis e espaços extras
void clean_log_message(char *message) {
    char *src = message;
    char *dst = message;
    
    while (*src) {
        if (isprint((unsigned char)*src) || isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';

    // Remove espaços em branco no início e no final
    char *start = message;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // Mover a string limpa para o início
    memmove(message, start, strlen(start) + 1);

    // Remove espaços em branco no final
    char *end = message + strlen(message) - 1;
    while (end > message && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
}

int make_decision(const char *log_message, DecisionNode *root) {
    char clean_message[256];
    strncpy(clean_message, log_message, sizeof(clean_message) - 1);
    clean_message[sizeof(clean_message) - 1] = '\0';  // Garantir null-terminado

    // Limpar a mensagem de log
    clean_log_message(clean_message);

    // Buscar a decisão na árvore de decisão
    int action = get_decision(root, clean_message);

    // Realiza a ação com base no código da decisão
    switch(action) {
        case ACTION_RESTART:
            printf(ANSI_COLOR_RED "Executando ação: Reiniciar o sistema\n" ANSI_RESET_ALL);
            break;
        case ACTION_SHUTDOWN:
            printf(ANSI_COLOR_RED "Executando ação: Desligar o sistema\n" ANSI_RESET_ALL);
            break;
        case ACTION_ALERT:
            printf(ANSI_COLOR_YELLOW "Executando ação: Enviar alerta ao administrador\n" ANSI_RESET_ALL);
            break;
        case ACTION_LOG:
            printf(ANSI_COLOR_GREEN "Registrando log de erro\n" ANSI_RESET_ALL);
            break;
        case ACTION_REPAIR:
            printf(ANSI_COLOR_GREEN "Executando ação: Reparar disco\n" ANSI_RESET_ALL);
            break;
        case ACTION_SLOWDOWN:
            printf(ANSI_COLOR_YELLOW "Executando ação: Reduzir uso de CPU\n" ANSI_RESET_ALL);
            break;
        default:
            printf("Ação não reconhecida.\n");
            return 1;
    }

    return 0;
}