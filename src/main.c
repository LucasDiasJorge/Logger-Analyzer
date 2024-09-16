#include "lib/decision_node.h"
#include "lib/error_map.h"
#include <signal.h>
#include <pthread.h>


pthread_t *threads;  // Global variable for threads
size_t num_threads = 0;  // Global number of threads
char **directories;  // Global directories

// Estrutura que armazena os dados de cada thread
typedef struct {
    char *filename;
    const char *target_string;
    const char *command;
    DecisionNode *root;
} agent;

void cleanup_and_exit(int signal)
{
    printf("\nReceived signal %d, cleaning up...\n", signal);

    // Cancel all threads
    for (size_t i = 0; i < num_threads; i++) {
        pthread_cancel(threads[i]);
    }

    // Wait for all threads to finish
    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free directories and threads
    free_directories(directories, num_threads);
    free(threads);

    // Exit the program
    exit(0);
}

void setup_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = cleanup_and_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) != 0) {
        perror("Error setting up signal handler");
        exit(1);
    }
}

void debugging(const agent *agent_data)
{
    printf("\n+--------------------------------------------------------------+\n");
    printf("|                        [ Agent Data ]                        |\n");
    printf("+--------------------------------------------------------------+\n");
    printf("|   FILE Name : %-46s |\n", agent_data->filename);
    printf("+--------------------------------------------------------------+\n\n");
}

void *thread_function(void *arg)
{
    agent *data = (agent *)arg;
    tail_file(data->filename, data->root);
    free(data->filename);  // Free allocated memory for filename
    free(data);  // Free allocated memory for agent data
    return NULL;
}

int read_directories(char ***directories, size_t *num_directories)
{
    char buffer[256];  // Buffer to store user input

    printf("Enter the number of directories: ");
    if (scanf("%zu", num_directories) != 1) {
        fprintf(stderr, "Error reading number of directories.\n");
        return 1;
    }

    *directories = malloc(*num_directories * sizeof(char *));
    if (*directories == NULL) {
        fprintf(stderr, "Error allocating memory for directories.\n");
        return 1;
    }

    for (size_t i = 0; i < *num_directories; i++) {
        printf("Enter the path for directory %zu: ", i);
        if (scanf("%255s", buffer) != 1) {
            fprintf(stderr, "Error reading directory path.\n");
            return 1;
        }
        
        (*directories)[i] = strdup(buffer);  // Allocate and copy string
        if ((*directories)[i] == NULL) {
            fprintf(stderr, "Error allocating memory for directory.\n");
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
    free(directories);  // Free memory allocated for directories array
}

int main() {

    // Setup signal handler to intercept CTRL+C
    setup_signal_handler();

    // Inicializa o mapa de erros
    initialize_error_map();

    // Constrói a árvore de decisão
    DecisionNode *decision_tree = build_decision_tree();

    // Read directories from user
    if (read_directories(&directories, &num_threads) != 0) {
        return 1;  // If failure to read or allocate, exit program
    }

    // Dynamically allocate array of threads
    threads = malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL) {
        fprintf(stderr, "Error allocating memory for threads.\n");
        free_directories(directories, num_threads);  // Free directories before exit
        return 1;
    }

    for (size_t i = 0; i < num_threads; i++) {
        const char *newest_file = get_newest_file(directories[i]);

        if (newest_file == NULL || newest_file[0] == '\0') {
            fprintf(stderr, "No valid file found in directory: %s\n", directories[i]);
            free_directories(directories, num_threads);  // Free directories before exit
            free(threads);  // Free threads before exit
            return 1;
        }

        // Dynamically allocate agent to avoid race conditions
        agent *agent_data = malloc(sizeof(agent));
        if (agent_data == NULL) {
            fprintf(stderr, "Error allocating memory for agent data.\n");
            free_directories(directories, num_threads);  // Free directories before exit
            free(threads);  // Free threads before exit
            return 1;
        }

        agent_data->filename = strdup(newest_file);
        DecisionNode *root = build_decision_tree();
        agent_data->root = root;

        if (pthread_create(&threads[i], NULL, thread_function, agent_data) != 0) {
            perror("Failed to create thread");
            free(agent_data->filename);  // Free filename before freeing agent data
            free(agent_data);
            free_directories(directories, num_threads);  // Free directories before exit
            free(threads);  // Free threads before exit
            return 1;
        }

        debugging(agent_data);  // Pass reference to agent_data
    }

    // Wait for all threads to finish
    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free directories and threads after threads have finished
    free_directories(directories, num_threads);
    free(threads);

    return 0;
    
}
