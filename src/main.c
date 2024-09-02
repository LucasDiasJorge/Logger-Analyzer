#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define TARGET_STRING "your_target_string"

void tail_file(const char *filename) {
    FILE *file;
    char buffer[BUFFER_SIZE];
    long position;

    while (1) {
        file = fopen(filename, "r");
        if (!file) {
            perror("Failed to open file");
            return;
        }

        // Move to the end of the file
        fseek(file, 0, SEEK_END);
        position = ftell(file);

        // Read file in reverse to find the last line
        while (position > 0) {
            fseek(file, --position, SEEK_SET);
            if (fgetc(file) == '\n' && position != ftell(file) - 1) {
                break;
            }
        }

        // Read and process the last line
        if (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            printf("Last line: %s\n", buffer);

            // Compare the last line with the target string
            if (strstr(buffer, TARGET_STRING) != NULL) {
                printf("Found the target string: %s\n", TARGET_STRING);
            }
        }

        fclose(file);
        sleep(1); // Sleep for a second before checking for new lines
    }
}

int main() {
    const char *filename = "to-read/your_file.txt"; // Replace with your file path
    tail_file(filename);

    return 0;
}
