#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "/home/user/Documentos/Logger-Analyzer/lib/file_scanner.h"

void* thread_function(void* arg) {
    thread_data_t *data = (thread_data_t *)arg;
    tail_file(data->filename, data->target_string, data->command);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) { // Expect 3 arguments: target string and action command
        fprintf(stderr, "Usage: %s <target_string> <action_command>\n", argv[0]);
        return 1;
    }

    const char *directories[4] = {
        "output/machine-zero",
        "output/machine-one",
        "output/machine-two",
        "output/machine-three"
    };

    const char *target_string = argv[1];
    const char *command = argv[2];

    pthread_t threads[4];
    thread_data_t thread_data[4];

    for (int i = 0; i < 4; i++) {
        // Get the newest file from the directory
        const char* newest_file = get_newest_file(directories[i]);

        //printf("Newest file is: %s\n",newest_file);

        if (newest_file[0] == '\0') {
            fprintf(stderr, "No valid files found in directory: %s\n", directories[i]);
            return 1;
        }

        thread_data[i].filename = newest_file;
        thread_data[i].target_string = target_string;
        thread_data[i].command = command;

        if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
