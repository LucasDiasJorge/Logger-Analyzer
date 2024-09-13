#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include "decision_tree.h"

#define BUFFER_SIZE 1024

typedef struct {
    const char *filename;
    DecisionNode root;
} agent;

const char* get_newest_file(const char* dirpath);

void tail_file(const char *filename, DecisionNode root);

#endif