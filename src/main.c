#include "/home/user/Documentos/Logger-Analyzer/lib/file_scanner.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <target_string>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *target_string = argv[2];

    tail_file(filename, target_string);

    return 0;
}
