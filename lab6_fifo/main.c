#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main() {
    const char* fifoName = "/tmp/myfifo";
    pid_t pid;
    char buff[100];

    if (mkfifo(fifoName, 0666) == -1) {
        perror("fifo err");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        int fifo = open(fifoName, O_RDONLY);

        ssize_t bytesRead = read(fifo, buff, 100);
        if (bytesRead == -1) {
            perror("Read from pipe failed");
            exit(EXIT_FAILURE);
        }
        sleep(5);
        time_t now = time(NULL);
        printf("got: %s", buff);
        printf("child time: %schild pid: %d", ctime(&now), getpid());
    } else {
        int fifo = open(fifoName, O_WRONLY);
        time_t now = time(NULL);
        char msg[100];
        sprintf(msg, "parent time: %sparent pid: %d\n", ctime(&now), getpid());

        if (write(fifo, msg, 100) == -1) {
            perror("Write to pipe failed");
            exit(EXIT_FAILURE);
        }
        sleep(10);
    }

    return 0;
}