#undef stderr
#define stderr stderr

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "buffer.h"

void *producer(void *param)
{
    buffer_item item;
    while (1)
    {
        // Sleep for a random period of time
        sleep(rand() % 3 + 1);
        // Generate a random number
        item = rand();
        if (insert_item(item))
            fprintf(stderr, "report error condition");
        else
            printf("producer produced %d\n", item);
    }
}

void *consumer(void *param)
{
    buffer_item item;
    while (1)
    {
        // Sleep for a random period of time
        sleep(rand() % 3 + 1);
        if (remove_item(&item))
            fprintf(stderr, "report error condition");
        else
            printf("consumer consumed %d\n", item);
    }
}

int main(int argc, char *argv[])
{
    // Get command line arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <sleep time> <#producer threads> <#consumer threads>\n", argv[0]);
        return 1;
    }
    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    // Initialize buffer
    initialize_buffer();

    // Create producer threads
    pthread_t producers[num_producers];
    for (int i = 0; i < num_producers; i++)
        pthread_create(&producers[i], NULL, producer, NULL);

    // Create consumer threads
    pthread_t consumers[num_consumers];
    for (int i = 0; i < num_consumers; i++)
        pthread_create(&consumers[i], NULL, consumer, NULL);

    // Sleep
    sleep(sleep_time);

    return 0;
}
