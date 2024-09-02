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
    
    if (argc != 7) { // Expect 7 arguments: program name, 4 filenames, target string, and action command
        fprintf(stderr, "Usage: %s <filename1> <filename2> <filename3> <filename4> <target_string> <action_command>\n", argv[0]);
        return 1;
    }

    const char *filenames[4] = {argv[1], argv[2], argv[3], argv[4]};
    const char *target_string = argv[5];
    const char *command = argv[6];

    pthread_t threads[4];
    thread_data_t thread_data[4];

    for (int i = 0; i < 4; i++) {
        thread_data[i].filename = filenames[i];
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