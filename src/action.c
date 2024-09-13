#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lib/decision_tree.h"

#define ANSI_RESET_ALL          "\x1b[0m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_GREEN        "\x1b[32m"

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

// Funcao que realiza a tomada de decisao
int make_decision(const char *log_message, DecisionNode *root) {
    char clean_message[256];
    strncpy(clean_message, log_message, sizeof(clean_message) - 1);
    clean_message[sizeof(clean_message) - 1] = '\0';  // Garantir null-terminado

    // Limpar a mensagem de log
    clean_log_message(clean_message);

    const char *command = NULL;

    // Verifique a mensagem do log para decidir o comando
    if (strstr(clean_message, "Falha de segmentacao") != NULL) {
        command = "RESTART"; // Exemplo de comando para reiniciar
    } 
    else if (strstr(clean_message, "erro de memoria") != NULL) {
        command = "SHUTDOWN"; // Comando para desligar
    } 
    else if (strstr(clean_message, "alerta de seguranca") != NULL) {
        command = "ALERT"; // Comando para enviar um alerta
    } 
    else {
        command = "LOG"; // Padrão: log de erro
    }

    // Realiza a ação com base na decisão
    if (command) {
        if (strcmp(command, "RESTART") == 0) {
            printf(ANSI_COLOR_RED "Executando ação: Reiniciar o sistema\n" ANSI_RESET_ALL);
            // Aqui você pode colocar o comando real para reiniciar
            // Exemplo: system("sudo reboot");
        } 
        else if (strcmp(command, "SHUTDOWN") == 0) {
            printf(ANSI_COLOR_RED "Executando ação: Desligar o sistema\n" ANSI_RESET_ALL);
            // Exemplo: system("sudo shutdown now");
        } 
        else if (strcmp(command, "ALERT") == 0) {
            printf(ANSI_COLOR_YELLOW "Executando ação: Enviar alerta ao administrador\n" ANSI_RESET_ALL);
            // Enviar alerta (Exemplo: e-mail)
        } 
        else if (strcmp(command, "LOG") == 0) {
            printf(ANSI_COLOR_GREEN "Registrando log de erro\n" ANSI_RESET_ALL);
            // Registrar log de erro (Exemplo: escrever em um arquivo de log)
        }
        return 0;
    }
    
    printf("Comando não reconhecido.\n");
    return 1;
}
