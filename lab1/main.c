#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#include "text.h"

struct Opts {
    char* path;
    bool isShowAll;
    bool isShowList;
} typedef Opts;

Opts retrieveArgs(int argc, char* argv[]) {
    Opts opts = {NULL, false, false};
    while (getopt(argc, argv, "al") != -1) {
        switch (optopt) {
            case 'a':
                opts.isShowAll = true;
                break;
            case 'l':
                opts.isShowList = true;
                break;
        }
    }
    opts.path = argv[optind];
    optind++;
    optreset = 1;
    while (getopt(argc, argv, "al") != -1) {
        switch (optopt) {
            case 'a':
                opts.isShowAll = true;
                break;
            case 'l':
                opts.isShowList = true;
                break;
        }
    }
    if (opts.path == NULL) {
        opts.path = ".";
    }
    return opts;
}

struct stat getFileStat(const char *path, const char *fileName) {
    struct stat fileStat;

    char *relPath = calloc(strlen(path) + strlen(fileName), sizeof(char));
    sprintf(relPath, "%s/%s", path, fileName);

    if (stat(relPath, &fileStat) != 0) {
        fprintf(stderr, "Error: Cannot get lstat for '%s'\n", fileName);
        exit(EXIT_FAILURE);
    }

    free(relPath);

    return fileStat;
}

void modeToString(mode_t mode, char *str) {
    if (S_ISREG(mode)) str[0] = '-';
    else if (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';

    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';

    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';

    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0';
}

void printListString(const char *fileName, struct stat fileStat) {
    char modeString[11];
    modeToString(fileStat.st_mode, modeString);

    const char *owner = getpwuid(fileStat.st_uid)->pw_name;
    const char *group = getgrgid(fileStat.st_gid)->gr_name;

    char timeStr[100];
    struct tm *time = localtime(&fileStat.st_mtimespec.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", time);

    printf("%-10s %3d %-8s %-8s %5lld %s %s\n", modeString, fileStat.st_nlink, owner, group, fileStat.st_size, timeStr,
           fileName);
}

void listDirectory(const char *path, bool isAll, bool isShowList) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Error: Cannot open directory '%s'\n", path);
        exit(EXIT_FAILURE);
    }

    if (isShowList) {
        struct stat s;
        stat(path, &s);
        printf("total %lld\n", s.st_blocks);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !isAll) {
            continue;
        }

        struct stat fileStat = getFileStat(path, entry->d_name);
        Color color;
        switch (entry->d_type) {
            case DT_DIR:
                color = blueColor;
                break;
            case DT_LNK:
                color = cyanColor;
                break;
            default:
                color = noColor;
        }

        if (S_ISREG(fileStat.st_mode) && (fileStat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
            color = greenColor;
        }

        char *coloredText = makeColorful(entry->d_name, color);
        if (isShowList) {
            printListString(coloredText, fileStat);
        } else {
            printf("%s ", coloredText);
        }
        free(coloredText);
    }

    if (closedir(dir) == -1) {
        fprintf(stderr, "Error: Cannot close directory '%s'\n", path);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {
    Opts opts = retrieveArgs(argc, argv);
    listDirectory(opts.path, opts.isShowAll, opts.isShowList);
}