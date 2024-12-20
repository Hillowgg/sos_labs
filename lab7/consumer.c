#include <stdio.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

int main() {
    sem_t* sem = sem_open("/lab7_sem1333333111", 0, 0777, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

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
        printf("____________producer___________\n%s", shmPtr);
        printf("____________consumer___________\ntime: %spid: %d\n", ctime(&now), getpid());
        sleep(1);
    };

    return EXIT_SUCCESS;
}