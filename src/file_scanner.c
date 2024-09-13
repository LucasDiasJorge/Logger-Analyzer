#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "lib/decision_tree.h"
#include "lib/action.h"

#define ANSI_BACKGROUND_WHITE_COLOR_BLACK   "\x1b[47m\x1b[30m"
#define ANSI_BACKGROUND_YELLOW_COLOR_BLACK  "\x1b[43m\x1b[30m"
#define ANSI_BOLD_TEXT                      "\x1b[1m"
#define ANSI_RESET_ALL                      "\x1b[0m"

#define BUFFER_SIZE 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

// Estrutura que armazena os dados de cada thread
typedef struct {
    char *filename;
    const char *target_string;
    const char *command;
    DecisionNode *root;
} agent;

// Função para obter o arquivo mais recente em um diretório
char* get_newest_file(const char* dirpath) {
    DIR* dir = opendir(dirpath);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    struct stat file_stat;
    char filepath[PATH_MAX];
    time_t newest_time = 0;
    char *newest_file = NULL;

    while ((entry = readdir(dir)) != NULL) {
        // Ignora "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
        if (stat(filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            if (file_stat.st_mtime > newest_time) {
                newest_time = file_stat.st_mtime;
                free(newest_file);  // Libera o nome do arquivo previamente alocado
                newest_file = strdup(filepath);  // Duplica o caminho do arquivo
            }
        }
    }

    closedir(dir);
    return newest_file;
}

// Função para imprimir o tempo atual
void print_current_time() {
    time_t now;
    struct tm *info;
    char time_buffer[80];

    time(&now);
    info = localtime(&now);

    // Formato: YYYY-MM-DD HH:MM:SS
    strftime(time_buffer, 80, "%Y-%m-%d %H:%M:%S", info);
    printf("[%s]", time_buffer);
}

// Função para limpar espaços extras e caracteres não imprimíveis do buffer
void clean_buffer(char *buffer) {
    char *src = buffer;
    char *dst = buffer;

    // Remove caracteres não imprimíveis e mantém apenas os imprimíveis e espaços
    while (*src) {
        if (isprint((unsigned char)*src) || isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';

    // Remove espaços em branco no início e no final
    char *start = buffer;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    memmove(buffer, start, strlen(start) + 1);

    char *end = buffer + strlen(buffer) - 1;
    while (end > buffer && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
}

// Função para monitorar um arquivo em busca de alterações
void tail_file(const char *filename, DecisionNode *root) {
    FILE *file;
    char buffer[BUFFER_SIZE];
    long position;
    int fd, wd;

    // Abre o arquivo para leitura
    file = fopen(filename, "r");
    if (!file) {
        perror("Falha ao abrir o arquivo");
        return;
    }

    // Define a posição inicial do arquivo para o final
    fseek(file, 0, SEEK_END);
    position = ftell(file);

    // Inicializa inotify
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init falhou");
        fclose(file);
        return;
    }

    // Adiciona um watch inotify para o arquivo
    wd = inotify_add_watch(fd, filename, IN_MODIFY);
    if (wd == -1) {
        perror("Falha ao adicionar inotify watch");
        close(fd);
        fclose(file);
        return;
    }

    while (1) {
        char event_buf[EVENT_BUF_LEN];
        int length = read(fd, event_buf, EVENT_BUF_LEN);

        if (length < 0) {
            perror("Erro ao ler o evento inotify");
            break;
        }

        // Processa as modificações no arquivo
        fseek(file, position, SEEK_SET);
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            clean_buffer(buffer);  // Limpa o buffer antes de processar

            if (strlen(buffer) > 0) {
                printf("\n");
                printf(ANSI_BACKGROUND_YELLOW_COLOR_BLACK ANSI_BOLD_TEXT "======================== [EVENT DETECTED] ========================");
                printf(ANSI_RESET_ALL "\n");
                print_current_time();
                printf("\n");
                printf("Buffer: \"%s\"\n", buffer);
                printf("Found in file: %s\n", filename);
                printf(ANSI_BACKGROUND_YELLOW_COLOR_BLACK ANSI_BOLD_TEXT "==================================================================");
                printf(ANSI_RESET_ALL "\n");

                sleep(1);

                // Executa a decisão com base no conteúdo do buffer (log)
                make_decision(buffer, root);

                // Atualiza a posição do arquivo
                position = ftell(file);
            }
        }
    }

    // Cleanup
    inotify_rm_watch(fd, wd);
    close(fd);
    fclose(file);
}
