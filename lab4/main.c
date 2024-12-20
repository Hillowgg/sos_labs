#include <sys/param.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Opts {
    char *path;
    char *modes;
} typedef Opts;

enum ModeType {
    MT_SET,
    MT_ADD,
    MT_REMOVE
} ModeType;

Opts retrieveArgs(int _, char *argv[]) {
    Opts opts = {NULL, NULL};
    opts.modes = argv[1];
    opts.path = argv[2];
    return opts;
}

mode_t parseModes(char *modesStr, enum ModeType *mt) {
    char *end;
    long modes = strtol(modesStr, &end, 8);
    if (*end == '\0') {
        if (modes < 0 || modes > 0777) {
            fprintf(stderr, "[int parsing] Error: wrong modes %s\n", modesStr);
            exit(EXIT_FAILURE);
        }
        *mt = MT_SET;
        return modes;
    }

    modes = 0;

    if (modesStr[0] == '+' || modesStr[0] == '-') {
        switch (modesStr[0]) {
            case '+':
                *mt = MT_ADD;
                break;
            case '-':
                *mt = MT_REMOVE;
                break;
        }

        for (int i = 1; i < strlen(modesStr); ++i) {
            switch (modesStr[i]) {
                case 'r':
                    modes |= S_IRUSR | S_IRGRP | S_IROTH;
                    break;
                case 'w':
                    modes |= S_IWUSR | S_IWGRP | S_IWOTH;
                    break;
                case 'x':
                    modes |= S_IXUSR | S_IXGRP | S_IXOTH;
                    break;
            }
        }
        return modes;
    }

    if (modesStr[0] != 'u' && modesStr[0] != 'g' && modesStr[0] != 'o') {
        fprintf(stderr, "[group parsing] Error: wrong modes %s\n", modesStr);
        exit(EXIT_FAILURE);
    }

    switch (modesStr[1]) {
        case '+':
            *mt = MT_ADD;
            break;
        case '-':
            *mt = MT_REMOVE;
            break;
    }

    if (modesStr[0] == 'u') {
        for (int i = 2; i < strlen(modesStr); ++i) {
            switch (modesStr[i]) {
                case 'r':
                    modes |= S_IRUSR;
                    break;
                case 'w':
                    modes |= S_IWUSR;
                    break;
                case 'x':
                    modes |= S_IXUSR;
                    break;
            }
        }
    } else if (modesStr[0] == 'g') {
        for (int i = 2; i < strlen(modesStr); ++i) {
            switch (modesStr[i]) {
                case 'r':
                    modes |= S_IRGRP;
                    break;
                case 'w':
                    modes |= S_IWGRP;
                    break;
                case 'x':
                    modes |= S_IXGRP;
                    break;
            }
        }
    } else if (modesStr[0] == 'o') {
        for (int i = 2; i < strlen(modesStr); ++i) {
            switch (modesStr[i]) {
                case 'r':
                    modes |= S_IROTH;
                    break;
                case 'w':
                    modes |= S_IWOTH;
                    break;
                case 'x':
                    modes |= S_IXOTH;
                    break;
            }
        }
    }

    return modes;
}

// remove
// 0 0 0
// 0 1 0
// 1 0 1
// 1 1 0


int main(int argc, char *argv[]) {
    Opts opts = retrieveArgs(argc, argv);

    enum ModeType mt;
    mode_t mode = parseModes(opts.modes, &mt);

    struct stat fs;
    stat(opts.path, &fs);

    if (mt == MT_ADD) {
        mode |= fs.st_mode;
    } else if (mt == MT_REMOVE) {
        mode = fs.st_mode & ~mode;
    }

    if (chmod(opts.path, mode) == -1) {
        perror("chmod error:");
        return EXIT_FAILURE;
    }

    return 0;
}