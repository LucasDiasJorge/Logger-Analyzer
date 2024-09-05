#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <errno.h>
#include "/home/user/Documentos/Logger-Analyzer/lib/action.h"

#define BUFFER_SIZE 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

void tail_file(const char *filename, const char *target_string, const char *command) {
    FILE *file;
    char buffer[BUFFER_SIZE];
    long position;
    int fd, wd;

    // Open file for reading
    file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Set file position to the end initially
    fseek(file, 0, SEEK_END);
    position = ftell(file);

    // Initialize inotify
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init failed");
        fclose(file);
        return;
    }

    // Add inotify watch for the file
    wd = inotify_add_watch(fd, filename, IN_MODIFY);
    if (wd == -1) {
        perror("Failed to add inotify watch");
        close(fd);
        fclose(file);
        return;
    }

    while (1) {
        char event_buf[EVENT_BUF_LEN];
        int length = read(fd, event_buf, EVENT_BUF_LEN);

        if (length < 0) {
            perror("Error reading inotify event");
            break;
        }

        // Process the file modifications
        fseek(file, position, SEEK_SET);
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            printf("Read line: %s\n", buffer);

            if (strstr(buffer, target_string) != NULL) {
                printf("Found the target string: %s\n", target_string);
                run_command(command);  // Trigger the action
            }

            position = ftell(file);
        }
    }

    // Cleanup
    inotify_rm_watch(fd, wd);
    close(fd);
    fclose(file);
}
