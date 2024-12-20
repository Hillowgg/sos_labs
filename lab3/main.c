#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void sigintHandler(int _) {
    printf("[pid: %d]sigint!\n", getpid());
    exit(0);
}
void sigtermHandler(int _) {
    printf("[pid: %d]sigterm!\n", getpid());
    exit(0);
}
void atExitHandler() {
    printf("[pid: %d]Exiting...\n", getpid());
}

int main(int argc, char *argv[]) {
    // SIGINT
    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        perror("SIGINT reg error");
        exit(EXIT_FAILURE);
    }

    // SIGTERM
    if (signal(SIGTERM, sigtermHandler) == SIG_ERR) {
        perror("SIGTERM reg error");
        exit(EXIT_FAILURE);
    }

    // on_exit
    if (atexit(atExitHandler) != 0) {
        perror("atexit reg error");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork() error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("[pid: %d]this is child process\n", getpid());
    } else {
        printf("[pid: %d]parent runned child process: %d\n", getpid(), pid);
        sleep(10);
    }
    return EXIT_SUCCESS;
}