#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Opts {
    char *path;
    char *pattern;
} typedef Opts;

Opts retrieveArgs(int argc, char *argv[]) {
    Opts opts = {NULL, NULL};
    opts.pattern = argv[1];
    if (argc > 2) {
        opts.path = argv[2];
    }
    return opts;
}

void grep(FILE* file, Opts opts) {
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, opts.pattern)) {
            printf("%s", line);
        }
    }
}

int main(int argc, char *argv[]) {
    Opts opts = retrieveArgs(argc, argv);

    if (argc < 2) {
        fprintf(stderr, "Error: argc < 1\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        grep(stdin, opts);
        return 0;
    }
    FILE *file = fopen(opts.path, "r");
    grep(file, opts);
    return 0;
}