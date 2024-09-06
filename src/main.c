#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "lib/file_scanner.h"

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
    // Assign thread ID as the actual kernel thread ID (PID for threads)
    tail_file(data->filename, data->target_string, data->command);
    return NULL;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    { // Expect 3 arguments: target string and action command
        fprintf(stderr, "Usage: %s <target_string> <action_command>\n", argv[0]);
        return 1;
    }

    const char *directories[4];
    char buffer[256];  // Buffer para armazenar a entrada do usuário

    for (int i = 0; i < 4; i++) {
        printf("Digite o caminho para o diretório %d: ", i);
        scanf("%255s", buffer);  // Lê até 255 caracteres para evitar overflow

        // Alocando memória dinamicamente para cada string
        directories[i] = strdup(buffer);  // Use strdup para copiar a string
    }

    // Exibir os diretórios para verificação
    for (int i = 0; i < 4; i++) {
        printf("Diretório %d: %s\n", i, directories[i]);
    }

    // Lembre-se de liberar a memória alocada dinamicamente
    for (int i = 0; i < 4; i++) {
        free((void*)directories[i]);
    }

    const char *target_string = argv[1];
    const char *command = argv[2];

    pthread_t threads[4];
    agent agent_data[4];

    for (int i = 0; i < 4; i++)
    {

        const char *newest_file = get_newest_file(directories[i]);

        if (newest_file[0] == '\0')
        {
            fprintf(stderr, "No valid files found in directory: %s\n", directories[i]);
            return 1;
        }

        agent_data[i].filename = newest_file;
        agent_data[i].target_string = target_string;
        agent_data[i].command = command;

        if (pthread_create(&threads[i], NULL, thread_function, &agent_data[i]) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }
        else
        {
            debugging(agent_data[i]);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
