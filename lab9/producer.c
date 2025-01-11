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
#include <signal.h>

sem_t *sem;
bool lockedSem = false;

void atExitHandler() {
    if (lockedSem) {
        printf("unlocking\n");
        sem_post(sem);
        sem_close(sem);
    }
    exit(0);
}

int main() {
    sem = sem_open("/lab7_sem1333333111", 0, 0777, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
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

    if (sem_trywait(sem) == -1) {
        printf("producer already running\n");
        sem_close(sem);
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
