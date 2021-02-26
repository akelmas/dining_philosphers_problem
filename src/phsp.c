/*
@author : akelmas
This program basically emulates a number of threads sharing same resources.
Each thread represents a philosopher.
The program accepts 7 arguments

the program calcualates average time passed during philosophers thinking

ARG         DESC
num_phsp    how many thread will be emulated
min_think   the minimum amount of time (sec) that a thread waits before trying to access memory
max_think   the maximum amount  of time (sec) that a thread waits. if a thread reaches maximum wait it immediately tries to access memory
min_dine    the time dining takes at minimum
max_dine    the maximum time until the thread state changes from hungry to thinking
dst         determines the type of waiting amount. 
num         cycle count that how many times threads repeat the process.

*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#define HUNGRY 0
#define THINKING 1
#define EATING 2
int num_phsp=5;
int min_think=1;
int max_think=2;
int min_dine=1;
int max_dine=2;
char dst[12]="exponential";
int num = 1;

int phil_state[27];
int phil_ids[27];

sem_t mutex;//protect console
sem_t S[27];
void *philospher(void *args)
{
    //Each philosopher should continue to think, eat or stay hungry
    //for <num> times

    int *i = args;
    //eating and thinking durations will be measured to find hunger duration

    long long int hungry_state_duration[num];

    struct timeval start, end;

    //record the current time

    for (int k = 0; k < num; k++)
    {

        //start thinking
        phil_state[*i] = THINKING;
        sem_wait(&mutex);
        printf("Philosopher %d : THINKING\n", *i);
        sem_post(&mutex);
        int lambda = (min_think + max_think) / 2;
        int duration_think = max_dine + 1;
        if (strcmp("exponential", dst) == 0)
        {
            while (duration_think < min_think || duration_think > max_think)
            {
                duration_think = -lambda * log(1 - ((double)(rand() % 1000) / 1000));
            }
        }
        else
        {
            duration_think = min_think + (rand() % (max_think - min_think));
        }
        usleep(duration_think * 1000000);
        phil_state[*i] = HUNGRY;
        gettimeofday(&start, NULL); //hunger starts here
        sem_wait(&mutex);
        printf("Philosopher %d : HUNGRY\n", *i);
        sem_post(&mutex);

        //check neighbour philosophers and take chopsticks
        if (phil_state[(*i + 4) % num_phsp] != EATING && phil_state[(*i + 1) % num_phsp] != EATING)
        {
            phil_state[*i] = EATING;
            sem_wait(&mutex);
            printf("Philosopher %d : EATING\n", *i);
            sem_post(&mutex);
        }
        else
        {
            //wait for chopsticks
            sem_wait(&mutex);
            printf("Philosopher %d : EATING\n", *i);
            sem_post(&mutex);
            phil_state[*i] = EATING;
        }
        //hunger ends here
        gettimeofday(&end, NULL);

        long hunger_duration = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));

        hungry_state_duration[k] = hunger_duration;

 
        //release mutex after taking chopsticks
        lambda = (max_dine + min_dine) / 2;
        int duration_dine = max_dine + 1;
        if (strcmp("exponential", dst) == 0)
        {
            while (duration_dine < min_dine || duration_dine > max_dine)
            {
                duration_dine = -lambda * log(1 - ((double)(rand() % 1000) / 1000));
            }
        }
        else
        {
            duration_dine = min_dine + (rand() % (max_dine - min_dine));
        }
        usleep(duration_dine * 1000000);
        
    }
    ///////////////////STATISTICS///////////////////
    double sum = 0;
    for (int j = 0; j < num; j++)
    {
        /* code */
        sum += hungry_state_duration[j];
    }
    double average = sum / num;
    //calculate standart deviation
    sum = 0;
    for (int j = 0; j < num; j++)
    {
        /* code */
        sum += pow(hungry_state_duration[j] - average, 2);
    }
    double std_deviation = sqrt(sum / num);

    printf("Average duration of hungry state for Philosopher %d : %fms\n", *i, average/1000);
    printf("Standard deviation of hungry state for Philosopher %d : %f\n", *i, std_deviation);
}

int main(int argc, char *argv[])
{
    srand(time(0));

    if (argc != 8)
    {
        puts("Some arguments missing...\nLoading defaults");
    }else{
        num_phsp = atoi(argv[1]);
        min_think = atoi(argv[2]);
        max_think = atoi(argv[3]);
        min_dine = atoi(argv[4]);
        max_dine = atoi(argv[5]);
        sprintf(dst, "%s", argv[6]);
        num = atoi(argv[7]);
    }

    int i;
    pthread_t thread_id[num_phsp];
    sem_init(&mutex, 0, 1);
    for (i = 0; i < num_phsp; i++)
    {
        phil_ids[i] = i;
    }
    for (i = 0; i < num_phsp; i++)
    {
        sem_init(&S[i], 0, 0);
    }
    for (i = 0; i < num_phsp; i++)
    {
        pthread_create(&thread_id[i], NULL, philospher, &phil_ids[i]);
    }
    for (i = 0; i < num_phsp; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
}
