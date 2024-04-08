#include "buffer.h"
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty; // Semaphore to track empty slots in the buffer
sem_t full;  // Semaphore to track filled slots in the buffer

void initialize_buffer()
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE); // Initialize empty semaphore to BUFFER_SIZE
    sem_init(&full, 0, 0);            // Initialize full semaphore to 0
}

int insert_item(buffer_item item)
{
    sem_wait(&empty); // Wait for an empty slot in the buffer
    pthread_mutex_lock(&mutex);
    // Critical section
    // Insert item into buffer
    pthread_mutex_unlock(&mutex);
    sem_post(&full); // Increment the count of filled slots in the buffer
    return 0;        // Success
}

int remove_item(buffer_item *item)
{
    sem_wait(&full); // Wait for a filled slot in the buffer
    pthread_mutex_lock(&mutex);
    // Critical section
    // Remove item from buffer
    pthread_mutex_unlock(&mutex);
    sem_post(&empty); // Increment the count of empty slots in the buffer
    return 0;         // Success
}
