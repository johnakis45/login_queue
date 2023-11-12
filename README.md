This program simulates a study hall scenario where students enter a study room to study for a random amount of time. The study hall has a maximum capacity of 8 students, and there is also a waiting room for additional students. The program uses pthreads and semaphores for synchronization.

Here's a step-by-step breakdown:

Initialization:
The program initializes semaphores (semaphore_study, semaphore_wait, semaphore_empty) and defines constants (MAX_CAPACITY, NUM_STUDENTS).

User Input:
Asks the user to input the number of students (getNumberOfStudents function).
Ensures that the number of students is between 20 and 40.

Thread Creation:
Creates an array of pthreads (thread) with the size of the specified number of students.
Initializes semaphores with appropriate initial values.

Thread Routine:
Each thread represents a student (routine function).
Students attempt to enter the waiting room, and if the study room is full, they wait in the waiting room.
Once the study room is empty,  students enter and study for a random amount of time.
After studying, students leave the study room.
The program uses semaphores to control access to the study room and waiting room.

Thread Joining:
Waits for all threads to finish using pthread_join.

Cleanup:
Destroys the semaphores.

How to Compile:
Compile the program using a C compiler (e.g., gcc).

Run make all to compile the program.
Run make run to run the program.
Run make clean to remove the executable file.
