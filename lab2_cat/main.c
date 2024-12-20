#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Opts {
    char *path;
    bool isNumberLines;
    bool isNumberNonEmptyLines;
    bool isDisplayDollar;
} typedef Opts;

Opts retrieveArgs(int argc, char *argv[]) {
    Opts opts = {NULL, false, false, false};
    while (getopt(argc, argv, "nbE") != -1) {
        switch (optopt) {
            case 'n':
                opts.isNumberLines = true;
                break;
            case 'b':
                opts.isNumberNonEmptyLines = true;
                break;
            case 'E':
                opts.isDisplayDollar = true;
                break;
        }
    }
    opts.path = argv[optind];
    optind++;
    optreset = 1;
    while (getopt(argc, argv, "nbE") != -1) {
        switch (optopt) {
            case 'n':
                opts.isNumberLines = true;
                break;
            case 'b':
                opts.isNumberNonEmptyLines = true;
                break;
            case 'E':
                opts.isDisplayDollar = true;
                break;
        }
    }
    if (opts.path == NULL) {
        fprintf(stderr, "Error: Cannot open empty file\n");
        exit(EXIT_FAILURE);
    }
    return opts;
}

void readFile(FILE *file, Opts opts) {
    char line[2048];
    int lineNumber = 1;
    int nonEmptyLineNumber = 1;

    while (fgets(line, sizeof(line), file)) {
        bool isEmpty = true;
        for (int i = 0; line[i] != 0; ++i) {
            if (!isspace(line[i])) {
                isEmpty = false;
                break;
            }
        }

        if (opts.isDisplayDollar) {
            unsigned long l = strlen(line);
            line[l - 1] = '$';
            line[l] = '\n';
            line[l + 1] = 0;
        }

        if (opts.isNumberNonEmptyLines && !isEmpty) {
            printf("%5d  %s", nonEmptyLineNumber, line);
            nonEmptyLineNumber++;
        } else if (opts.isNumberLines) {
            printf("%5d  %s", lineNumber, line);
            lineNumber++;
        } else {
            printf("%s", line);
        }

    }
}

int main(int argc, char *argv[]) {
    Opts opts = retrieveArgs(argc, argv);
    FILE *file = fopen(opts.path, "r");
    readFile(file, opts);

    return 0;
}