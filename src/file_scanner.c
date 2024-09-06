#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include "/home/user/Documentos/Logger-Analyzer/lib/action.h"

#define BUFFER_SIZE 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

// Function to get the newest file in a directory
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
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
        if (stat(filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            if (file_stat.st_mtime > newest_time) {
                newest_time = file_stat.st_mtime;
                free(newest_file);  // Free any previously allocated file name
                newest_file = strdup(filepath);  // Duplicate the file path
            }
        }
    }

    closedir(dir);
    return newest_file;
}

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
        printf("Reading file: %s\n",filename);
        char event_buf[EVENT_BUF_LEN];
        int length = read(fd, event_buf, EVENT_BUF_LEN);

        if (length < 0) {
            perror("Error reading inotify event");
            break;
        }

        // Process the file modifications
        fseek(file, position, SEEK_SET);
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            //printf("Read line: %s\n", buffer);

            if (strstr(buffer, target_string) != NULL) {
                //printf("Found the target string: %s\n", target_string);
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
