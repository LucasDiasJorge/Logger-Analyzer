#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include "lib/file_scanner.h"

pthread_t threads[4]; // Variável global para as threads
size_t num_threads = 4; // Número de threads global
char *directories[4]; // Diretórios globais

void cleanup_and_exit(int signal)
{
    printf("\nRecebido sinal %d, finalizando...\n", signal);

    // Cancela todas as threads
    for (size_t i = 0; i < num_threads; i++) {
        pthread_cancel(threads[i]);
    }

    // Libera os diretórios
    free_directories(directories, num_threads);

    // Finaliza o programa
    exit(0);
}

void setup_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = cleanup_and_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) != 0) {
        perror("Erro ao configurar manipulador de sinal");
        exit(1);
    }
}

void debugging(agent agent_data)
{
    printf("\n+--------------------------------------------------------------+\n");
    printf("|                        [ Agent Data ]                        |\n");
    printf("+--------------------------------------------------------------+\n");
    printf("|   FILE Name : %-46s |\n", agent_data.filename);
    printf("|   Target    : %-46s |\n", agent_data.target_string);
    printf("|   Command   : %-46s |\n", agent_data.command);
    printf("+--------------------------------------------------------------+\n\n");
}

void *thread_function(void *arg)
{
    agent *data = (agent *)arg;
    tail_file(data->filename, data->target_string, data->command);
    free(data); // Libera a memória alocada para os dados do agente
    return NULL;
}

int read_directories(char **directories, size_t num_directories)
{
    char buffer[256];  // Buffer para armazenar a entrada do usuário

    for (size_t i = 0; i < num_directories; i++) {
        printf("Digite o caminho para o diretório %zu: ", i);
        if (scanf("%255s", buffer) != 1) {
            fprintf(stderr, "Erro ao ler o caminho do diretório.\n");
            return 1;
        }
        
        directories[i] = strdup(buffer);  // Aloca e copia a string
        if (directories[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memória para o diretório.\n");
            return 1;
        }
    }
    return 0;
}

void free_directories(char **directories, size_t num_directories)
{
    for (size_t i = 0; i < num_directories; i++) {
        free(directories[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) { // Espera 3 argumentos: target string e comando de ação
        fprintf(stderr, "Usage: %s <target_string> <action_command>\n", argv[0]);
        return 1;
    }

    // Configura o manipulador de sinal para interceptar CTRL+C
    setup_signal_handler();

    const size_t num_threads = 4;

    // Lê os diretórios do usuário
    if (read_directories(directories, num_threads) != 0) {
        return 1; // Se falhar ao ler ou alocar, termina o programa
    }

    const char *target_string = argv[1];
    const char *command = argv[2];

    for (size_t i = 0; i < num_threads; i++) {
        const char *newest_file = get_newest_file(directories[i]);

        if (newest_file == NULL || newest_file[0] == '\0') {
            fprintf(stderr, "Nenhum arquivo válido encontrado no diretório: %s\n", directories[i]);
            free_directories(directories, num_threads); // Libera os diretórios antes de sair
            return 1;
        }

        // Alocar dinamicamente o agente para evitar race conditions
        agent *agent_data = malloc(sizeof(agent));
        if (agent_data == NULL) {
            fprintf(stderr, "Erro ao alocar memória para os dados do agente.\n");
            free_directories(directories, num_threads); // Libera os diretórios antes de sair
            return 1;
        }

        agent_data->filename = strdup(newest_file);
        agent_data->target_string = target_string;
        agent_data->command = command;

        if (pthread_create(&threads[i], NULL, thread_function, agent_data) != 0) {
            perror("Falha ao criar thread");
            free(agent_data);
            free_directories(directories, num_threads); // Libera os diretórios antes de sair
            return 1;
        }

        debugging(*agent_data);
    }

    // Espera todas as threads terminarem
    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Libera os diretórios após as threads terminarem
    free_directories(directories, num_threads);

    return 0;
}
