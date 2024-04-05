#include <iostream>
#include <pthread.h>
#include <dispatch/dispatch.h>
#include <queue>
#include <unistd.h>
#include <cstdlib>

const int NUM_STUDENTS = 5; // Number of students
const int CHAIRS = 3;       // Number of chairs in the hallway

// Mutex and semaphores
pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;
dispatch_semaphore_t sem_students;
dispatch_semaphore_t sem_ta;

std::queue<int> waiting_students_queue;                  // Queue of waiting students
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for the queue

bool ta_sleeping = true;
pthread_mutex_t ta_sleep_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to simulate programming
void programming()
{
    // Simulate programming by sleeping for a random time between 3 and 10 seconds
    usleep(rand() % 17000000 + 3000000); // Sleep for 3 to 20 seconds
}

// Function representing student behavior
void *student_behavior(void *id)
{
    int student_id = *((int *)id);

    while (true)
    {
        // Programming
        pthread_mutex_lock(&mutex_);
        std::cout << "Student " << student_id << " is programming." << std::endl;
        pthread_mutex_unlock(&mutex_);
        programming();

        // Seeking help
        pthread_mutex_lock(&mutex_);
        if (waiting_students_queue.size() < CHAIRS)
        {
            // Enqueue the student
            pthread_mutex_lock(&queue_mutex);
            waiting_students_queue.push(student_id);
            pthread_mutex_unlock(&queue_mutex);

            std::cout << "Student " << student_id << " is waiting for help." << std::endl;

            pthread_mutex_unlock(&mutex_);
            dispatch_semaphore_signal(sem_students);                // Notify TA
            dispatch_semaphore_wait(sem_ta, DISPATCH_TIME_FOREVER); // Wait for TA to help
        }
        else
        {
            pthread_mutex_unlock(&mutex_);
            pthread_mutex_lock(&mutex_);
            std::cout << "No empty chairs. Student " << student_id << " returns to programming." << std::endl;
            pthread_mutex_unlock(&mutex_);
        }
    }

    pthread_exit(NULL);
}

// // Function representing TA behavior
void *ta_behavior(void *)
{
    while (true)
    {
        // Wait for students or until TA is wakened up
        dispatch_semaphore_wait(sem_students, DISPATCH_TIME_FOREVER);

        // Set TA as awake
        pthread_mutex_lock(&ta_sleep_mutex);
        if (ta_sleeping)
        {
            std::cout << "TA is waking up." << std::endl;
            ta_sleeping = false;
        }
        pthread_mutex_unlock(&ta_sleep_mutex);

        // Dequeue the next student to help
        pthread_mutex_lock(&mutex_);
        int student_id = waiting_students_queue.front();
        waiting_students_queue.pop();
        pthread_mutex_unlock(&mutex_);

        // Get the ID of the student the TA is helping
        std::cout << "TA is helping Student " << student_id << "." << std::endl;
        usleep(rand() % 8000000 + 3000000); // Simulate helping a student
        std::cout << "TA finishes helping Student " << student_id << "." << std::endl;

        // Notify student that TA is available
        dispatch_semaphore_signal(sem_ta);

        // Check if there are more students waiting
        pthread_mutex_lock(&mutex_);
        if (waiting_students_queue.empty())
        {
            std::cout << "TA is going to sleep." << std::endl;
            ta_sleeping = true;
        }
        pthread_mutex_unlock(&mutex_);
    }

    pthread_exit(NULL);
}

int main()
{
    srand(time(NULL));

    // Initialize semaphores
    sem_students = dispatch_semaphore_create(0);
    sem_ta = dispatch_semaphore_create(0);

    // Create TA thread
    pthread_t ta_thread;
    pthread_create(&ta_thread, NULL, ta_behavior, NULL);

    // Create student threads
    pthread_t student_threads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    for (int i = 0; i < NUM_STUDENTS; ++i)
    {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student_behavior, &student_ids[i]);
    }

    // Join threads
    pthread_join(ta_thread, NULL);
    for (int i = 0; i < NUM_STUDENTS; ++i)
    {
        pthread_join(student_threads[i], NULL);
    }

    // Destroy semaphores and mutex
    dispatch_release(sem_students);
    dispatch_release(sem_ta);
    pthread_mutex_destroy(&mutex_);

    return 0;
}
