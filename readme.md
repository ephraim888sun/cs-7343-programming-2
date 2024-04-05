For each project, please submit
1. The source code of the project
2. Brief description of how to test your program
3. A sample Output (snapshot or a video of program running)







### Project 2: The Sleeping Teaching Assistant
A university computer science department has a teaching assistant (TA) who helps undergraduate students with their programming assignments during regular office hours. The TA's office is rather small and has room for only one desk with a chair and computer. There are three chairs in the hallway outside the office where students can sit and wait if the TA is currently helping another student. When there are no students who need help during office hours, the TA sits at the desk and takes a nap. If a student arrives during office hours and finds the TA sleeping, the student must awaken the TA to ask for help. If a student arrives and finds the TA currently helping another student, the student sits on one of the chairs in the hallway and waits. If no chairs are available, the student will come back at a later time.
Using POSIX threads, mutex locks, and semaphores, implement a solu- tion that coordinates the activities of the TA and the students. Details for this assignment are provided below.
The Students and the TA
Using Pthreads (Section 4.4.1), begin by creating n students where each student will run as a separate thread. The TA will run as a separate thread as well. Student threads will alternate between programming for a period of time and seeking help from the TA. If the TA is available, they will obtain help. Otherwise, they will either sit in a chair in the hallway or, if no chairs are available, will resume programming and will seek help at a later time. If a student arrives and notices that the TA is sleeping, the student must notify the TA using a semaphore. When the TA finishes helping a student, the TA must check to see if there are students waiting for help in the hallway. If so, the TA must help each of these students in turn. If no students are present, the TA may return to napping.
Perhaps the best option for simulating students programming—as well as the TA providing help to a student—is to have the appropriate threads sleep for a random period of time.
Coverage of POSIX mutex locks and semaphores is provided in Section 7.3. Consult that section for details.


Example Output
```
Student 3 is programming.
Student 3 is waiting for help.
TA is helping Student 3.
Student 5 is programming.
Student 5 is waiting for help.
TA is helping Student 5.
Student 1 is programming.
Student 1 is waiting for help.
Student 2 is programming.
Student 2 is waiting for help.
Student 4 is programming.
Student 4 is waiting for help.
TA is helping Student 1.
Student 3 is programming.
Student 3 is waiting for help.
TA is helping Student 2.
Student 5 is programming.
Student 5 is waiting for help.
TA is helping Student 4.
TA is helping Student 3.
Student 1 is programming.
Student 1 is waiting for help.
TA is helping Student 5.
Student 2 is programming.
Student 2 is waiting for help.
Student 3 is programming.
Student 3 is waiting for help.
TA is helping Student 1.
TA is helping Student 2.
Student 4 is programming.
Student 4 is waiting for help.
```


### Project 3: The Dining-Philosophers Problem
In Section 7.1.3, we provide an outline of a solution to the dining-philosophers problem using monitors. This project involves implementing a solution to this problem using either POSIX mutex locks and condition variables or Java condition variables. Solutions will be based on the algorithm illustrated in Figure 7.7.
Both implementations will require creating five philosophers, each identi- fied by a number 0..4. Each philosopher will run as a separate thread. Philoso- phers alternate between thinking and eating. To simulate both activities, have each thread sleep for a random period between one and three seconds.
I. POSIX
Thread creation using Pthreads is covered in Section 4.4.1. When a philosopher wishes to eat, she invokes the function
pickup forks (int philosopher_number)
where philosopher_number identifies the number of the philosopher wishing to eat. When a philosopher finishes eating, she invokes
return_forks (int philosopher_number)
Your implementation will require the use of POSIX condition variables, which are covered in Section 7.3.