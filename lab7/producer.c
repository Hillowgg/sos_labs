#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/errno.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

int lock;
bool lockedSem = false;

void atExitHandler() {
    if (lockedSem) {
        printf("unlocking\n");
        close(lock);
    }
    exit(0);
}

int main() {
    lock = open("my_lock", O_CREAT | O_RDWR, 0666);
    if (lock == -1) {
        perror("lock open");
        exit(EXIT_FAILURE);
    }

    // SIGINT
    if (signal(SIGINT, atExitHandler) == SIG_ERR) {
        perror("SIGINT reg error");
        exit(EXIT_FAILURE);
    }

    // SIGTERM
    if (signal(SIGTERM, atExitHandler) == SIG_ERR) {
        perror("SIGTERM reg error");
        exit(EXIT_FAILURE);
    }

    if (flock(lock, LOCK_EX | LOCK_NB) == -1) {
        printf("producer already running\n");
        close(lock);
        exit(EXIT_SUCCESS);
    }
    printf("locked\n");
    lockedSem = true;

    int shmId = shmget(12345, 100, IPC_CREAT | 0666);
    if (shmId == -1) {
        perror("Failed to connect to shared memory");
        exit(EXIT_FAILURE);
    }

    char *shmPtr = (char *) shmat(shmId, NULL, 0);
    if (shmPtr == (void *)-1) {
        perror("Failed to attach shared memory");
        exit(EXIT_FAILURE);
    }

    while (true) {
        time_t now = time(0);
        sprintf(shmPtr, "time: %spid: %d\n", ctime(&now), getpid());
        sleep(1);
    };

    return EXIT_SUCCESS;
}
