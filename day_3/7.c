#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NUMBERS_COUNT 10
#define SEM_KEY 1234

// Semaphore union for semctl
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void producer(int sem_id);
void consumer(int sem_id);

int main() 
{
    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) 
    {
        perror("Failed to create semaphore");
        exit(EXIT_FAILURE);
    }

    union semun su;
    su.val = 1;
    if (semctl(sem_id, 0, SETVAL, su) == -1) 
    {
        perror("Failed to initialize semaphore");
        semctl(sem_id, 0, IPC_RMID, 0); 
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) 
    {
        perror("Failed to fork");
        semctl(sem_id, 0, IPC_RMID, 0); 
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) 
    {
        producer(sem_id);
    } 
    else 
    {
        consumer(sem_id);
    }

    semctl(sem_id, 0, IPC_RMID, 0);

    return 0;
}

void producer(int sem_id) 
{
    struct sembuf sem_op;

    for (int i = 1; i <= NUMBERS_COUNT; i++) 
    {
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);

        printf("Producing number: %d\n", i);

        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);
    }

    printf("Producer finished.\n");
}

void consumer(int sem_id) 
{
    int sum = 0,i;
    struct sembuf sem_op;
    int producer_finished = 0;

    for(i=1;i <= NUMBERS_COUNT;i++) 
    {
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);
        sum += i;
	printf("consumed number %d: sum = %d\n",i,sum);
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);

        if (sum >= NUMBERS_COUNT * (NUMBERS_COUNT + 1) / 2) 
	{
            break;
        }
    }

    printf("Sum of numbers: %d\n", sum);
}
