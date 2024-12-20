#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main() {
    int pipeBe[2];
    pid_t pid;
    char buff[100];

    if (pipe(pipeBe) == -1) {
        perror("Pipe err");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(pipeBe[1]);

        ssize_t bytesRead = read(pipeBe[0], buff, 100);
        if (bytesRead == -1) {
            perror("Read from pipe failed");
            exit(EXIT_FAILURE);
        }
        sleep(5);
        time_t now = time(NULL);
        printf("got: %s", buff);
        printf("child time: %schild pid: %d", ctime(&now), getpid());
        close(pipeBe[0]);
    } else {
        close(pipeBe[0]);
        time_t now = time(NULL);
        char msg[100];
        sprintf(msg, "parent time: %sparent pid: %d\n", ctime(&now), getpid());

        if (write(pipeBe[1], msg, 100) == -1) {
            perror("Write to pipe failed");
            exit(EXIT_FAILURE);
        }
        close(pipeBe[1]);
        sleep(10);
    }

    return 0;
}
