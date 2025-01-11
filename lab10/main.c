#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


pthread_cond_t cond;
pthread_mutex_t mu;

void *read_thread(void* counter) {
    int *num = (int*)counter;
    while(1) {
        pthread_mutex_lock(&mu);
        pthread_cond_wait(&cond, &mu);
        printf("tid: %lu, counter: %d\n", pthread_self(), *num);
        pthread_mutex_unlock(&mu);
    }
}

void* write_thread(void* counter) {
    int *num = (int*)counter;
    while(1) {
        sleep(1);
        pthread_mutex_lock(&mu);
        (*num)++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mu);
    }
}

int main() {
    if (pthread_cond_init(&cond, NULL) != 0) {
        perror("Failed to create mutex");
        exit(1);
    }
    if (pthread_mutex_init(&mu, NULL) != 0) {
        perror("Failed to create mutex");
        exit(1);
    }
    int counter = 0;
    pthread_t threads[11];
    if (pthread_create(&threads[0], NULL, write_thread, &counter) != 0) {
        perror("Failed to create thread");
            exit(1);
    }
    for (int i = 1; i < 11; ++i) {
        if (pthread_create(&threads[i], NULL, read_thread, &counter) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }
    if (pthread_join(threads[0], NULL) != 0) {
        perror("Failed to join thread");
        exit(1);
    }

    if (pthread_cond_destroy(&cond) != 0) {
        perror("Failed to destroy mutex");
        exit(1);
    }
    if (pthread_mutex_destroy(&mu) != 0) {
        perror("Failed to destroy mutex");
        exit(1);
    }
    return 0;
}