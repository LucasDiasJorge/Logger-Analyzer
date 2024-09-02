#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void tail_file(const char *filename, const char *target_string) {
    FILE *file;
    char buffer[BUFFER_SIZE];
    long position;

    file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Move to the end of the file initially
    fseek(file, 0, SEEK_END);
    position = ftell(file);

    while (1) {
        // Move to the previous position to read the new data
        fseek(file, position, SEEK_SET);

        // Read and process the new lines
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            // Print the line for debug purposes
            printf("Read line: %s", buffer);

            // Compare the line with the target string
            if (strstr(buffer, target_string) != NULL) {
                printf("Found the target string: %s\n", target_string);
            }

            // Update the position for the next iteration
            position = ftell(file);
        }

        // Sleep for a second before checking for new lines
        sleep(5);

        // Reopen the file in case it has been modified or rotated
        file = freopen(filename, "r", file);
        if (!file) {
            perror("Failed to reopen file");
            return;
        }

        // Move to the last known position
        fseek(file, position, SEEK_SET);
    }

    fclose(file);
}
