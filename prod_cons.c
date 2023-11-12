#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_CAPACITY 8
#define NUM_STUDENTS 20

sem_t semaphore_study;
sem_t semaphore_wait;
sem_t semaphore_empty;
int studyRoom[8] = {0};
int waitingRoom[40] = {0};
int waiting_students = 0;
int studying_students = 0;
int left_students = 0;
int numberOfStudents;
int full = 0;


int random_int() {
    return 5 + rand() / (RAND_MAX / (15 - 5 + 1) + 1);
}

int getNumberOfStudents() {
    int n;
    do {
        printf("Please enter the number of students N:\n");
        scanf("%d", &n);
        if (n < 20 || n > 40) {
            printf("N must be between 20 and 40. Please try again.\n");
        }
    } while (n < 20 || n > 40);
    
    return n;
}

void printRooms(int studyRoom[], int waitingRoom[], int studyRoomSize, int waitingRoomSize) {
    printf("\nStudy room:   |");
    for (int i = 0; i < studyRoomSize; i++) {
        if (studyRoom[i] == 0) {
            printf("   |");
        } else {
            printf(" %d |", studyRoom[i]);
        }
    }
    printf("\n");
    printf("Waiting room: |");
    for (int i = 0; i < waitingRoomSize && (i < 8 || waitingRoom[i] != 0); i++) {
        if (waitingRoom[i] == 0) {
            printf("   |");
        } else {
            printf(" %d |", waitingRoom[i]);
        }
    }
    
    printf("\n");
}

int shiftLeft(int arr[], int size) {
    int nonZeroIndex = 0; 
    // Iterate through the array and move non-zero elements to the front
    for (int i = 0; i < size; i++) {
        if (arr[i] != 0) {
            arr[nonZeroIndex] = arr[i];
            nonZeroIndex++;
        }
    }
    // Fill the remaining elements with zeros
    for (int i = nonZeroIndex; i < size; i++) {
        arr[i] = 0;
    }
    return nonZeroIndex;
}

void* routine(void* arg) {
    int student_id = *(int*)arg;
    int index;
    printf("\nStudent %d was born.\n",student_id);
    sem_wait(&semaphore_wait);  // Try to acquire a spot in the waiting room
    waitingRoom[waiting_students] = student_id; // Add student to waiting room
    waiting_students++; // Increment the number of students in the waiting room
    if (studying_students == MAX_CAPACITY || full == 1)
    {   
        full = 1;
        printf("\nStudent %d cannot enter the study hall.It is full.\n", student_id);
        printRooms(studyRoom, waitingRoom, 8, 40); 
        
    }
    sem_post(&semaphore_wait);  // Allow the waiting students to enter the study hall

    
    if (full == 1)
    {
        sem_wait(&semaphore_empty);
        usleep(10);
    }


    sem_wait(&semaphore_study);  // Try to acquire a spot in the study room   
    printf("\nStudent %d entered the study room.\n", student_id);
    waitingRoom[0] = 0; // Remove student from waiting room
    waiting_students=shiftLeft(waitingRoom, 40);
    // Increment the number of students in the study room                                                                                                                                    sem_post(&semaphore_wait);    // Allow the waiting students to enter the study hall
    studyRoom[studying_students] = student_id; // Add student to study room
    index = studying_students; // Save the index of the student in the study room
    studying_students++; 
    if (studying_students == MAX_CAPACITY && full == 0)
    {   
        full = 1;
    }
    if(waiting_students>0 && full == 0 ){
         sem_post(&semaphore_empty);
    }
    printRooms(studyRoom, waitingRoom, 8, 40);
    // Simulate some studying time
    int time = random_int();
    sleep(time);

    // Student is done studying and leaves the study room
    studyRoom[index] = 0;
    
    printf("\nStudent %d exited the study hall after studying for %d secs.\n", student_id, time);
    printRooms(studyRoom, waitingRoom, 8, 40);

    left_students++;
    if (left_students == MAX_CAPACITY && full == 1)
    {
        //printf("\nSTUDY HALL IS EMPTY.\n");
        full = 0;
        left_students = 0;
        studying_students = 0;
        sem_post(&semaphore_empty);
    }
    sem_post(&semaphore_study);  // Release the spot in the study room
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    numberOfStudents = getNumberOfStudents();
    pthread_t thread[numberOfStudents];
    sem_init(&semaphore_study, 0, 8);
    sem_init(&semaphore_wait, 0, 1);
    sem_init(&semaphore_empty, 0, -1);
    for (int i = 0; i < numberOfStudents; i++)
    {
        int * a= malloc(sizeof(int));
        *a = i+1;
        usleep(10);
        if (pthread_create(&thread[i], NULL, &routine, a) != 0)
        {
           perror("Failed to create thread");
        }
        
    }
    for (int i = 0; i < numberOfStudents; i++)
    {
        if(pthread_join(thread[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    //printf("You entered: %d\n", numberOfStudents);
    printf("\nAll %d students studied.\n", numberOfStudents);
    sem_destroy(&semaphore_study);
    sem_destroy(&semaphore_wait);
    sem_destroy(&semaphore_empty);
    return 0;
}
