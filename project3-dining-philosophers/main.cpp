#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define LEFT_FORK(i) ((i) % NUM_PHILOSOPHERS)
#define RIGHT_FORK(i) (((i) + 1) % NUM_PHILOSOPHERS)

pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_cond_t can_eat[NUM_PHILOSOPHERS];

void pickup_forks(int philosopher_number)
{
    pthread_mutex_lock(&forks[LEFT_FORK(philosopher_number)]);
    pthread_mutex_lock(&forks[RIGHT_FORK(philosopher_number)]);
}

void return_forks(int philosopher_number)
{
    pthread_mutex_unlock(&forks[LEFT_FORK(philosopher_number)]);
    pthread_mutex_unlock(&forks[RIGHT_FORK(philosopher_number)]);
}

void *philosopher(void *param)
{
    int philosopher_number = *((int *)param);

    while (1)
    {
        printf("Philosopher %d is thinking.\n", philosopher_number);
        usleep((rand() % 3000000) + 1000000); // Sleep between 1 and 3 seconds

        pickup_forks(philosopher_number);

        printf("Philosopher %d is eating.\n", philosopher_number);
        usleep((rand() % 3000000) + 1000000); // Sleep between 1 and 3 seconds

        return_forks(philosopher_number);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_numbers[NUM_PHILOSOPHERS];

    srand(time(NULL));

    // Initialize mutex locks
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        philosopher_numbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutex locks
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
