#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

struct Opts {
    char* path;
    bool showAll;
    bool showList;
} typedef Opts;

Opts retrieveArgs(int argc, char* argv[]) {
    Opts opts = {"", false, false};
    while (getopt(argc, argv, "al") != -1) {
        switch (optopt) {
            case 'a':
                opts.showAll = true;
                break;
            case 'l':
                opts.showList = true;
                break;
        }
    }
    opts.path = argv[optind];
    optind++;
    optreset = 1;
    while (getopt(argc, argv, "al") != -1) {
        switch (optopt) {
            case 'a':
                opts.showAll = true;
                break;
            case 'l':
                opts.showList = true;
                break;
        }
    }
    return opts;
}

int main(int argc, char* argv[]) {
    Opts opts = retrieveArgs(argc, argv);
    printf("path:%s, showAll:%d, showList:%d", opts.path, opts.showAll, opts.showList);
}