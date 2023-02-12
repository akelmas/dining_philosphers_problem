#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PROJECT_NAME "dining_philosophers"

enum State {
  ST_THINKING,
  ST_HUNGRY,
  ST_EATING,
  ST_FULL,
};

typedef struct _Fork {
  sem_t lock;
} Fork;

typedef struct _Philosopher {
  pthread_t tid;
  int number;
  int max_cycle;
  enum State state;
  int dining_duration;
  int thinking_duration;
  double measured_hunger_duration;
  Fork *left;
  Fork *right;
} Philosopher;

typedef void (*ActionCallback)(Philosopher *);

Fork *g_forks;

double random_double(void) {
  unsigned int rand_uint = arc4random();
  return (double)rand_uint / UINT_MAX;
}

int exponential_random(int expected_value) {
  double uniform_random_value = 0;

  do {
    uniform_random_value = random_double();
  } while (uniform_random_value == 0 || uniform_random_value == 1);

  return (int)(-expected_value * log(uniform_random_value));
}

void think(Philosopher *philosopher) {
  usleep(philosopher->thinking_duration);
  philosopher->state = ST_HUNGRY;
}

void take_forks(Philosopher *philosopher) {
  sem_wait(&philosopher->left->lock);
  sem_wait(&philosopher->right->lock);
  philosopher->state = ST_EATING;
}

void eat(Philosopher *philosopher) {
  usleep(philosopher->dining_duration);
  philosopher->state = ST_FULL;
}

void release_forks(Philosopher *philosopher) {
  sem_post(&philosopher->left->lock);
  sem_post(&philosopher->right->lock);
  philosopher->max_cycle--;
  philosopher->state = ST_THINKING;
}

void perform(Philosopher *philosopher, ActionCallback action_callback,
             double *duration_us) {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  action_callback(philosopher);
  clock_gettime(CLOCK_MONOTONIC, &end);
  *duration_us = (end.tv_sec - start.tv_sec);
  *duration_us += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

void *run(void *arg) {
  Philosopher *philosopher = arg;

  if (philosopher->max_cycle) {
    while (philosopher->max_cycle) {
      switch (philosopher->state) {
      case ST_THINKING:
        think(philosopher);
        break;
      case ST_HUNGRY: {
        double duration;
        perform(philosopher, take_forks, &duration);
        philosopher->measured_hunger_duration += duration;
        break;
      }
      case ST_EATING:
        eat(philosopher);
        break;
      case ST_FULL:
        release_forks(philosopher);
        break;
      }
    }
  }

  return philosopher;
}

void print_help(void) {
  printf(
      "\n%s\n\n"    // usage
      "%20s : %s\n" // n
      "%20s : %s\n" // thinking_duration
      "%20s : %s\n" // dining_duration
      "%20s : %s\n" // cycle
      "\n%s\n",     // example
      "Usage : ./dining_philosophers n thinking_duration dining_duration cycle",
      "n", "number of philosophers to emulate", "thinking_duration",
      "expected time (us) for thinking", "dining_duration",
      "expected time (us) for dining", "cycle",
      "count to repeat cycle: THINKING->HUNGER->EATING",
      "Example : ./dining_philosophers 5 210 20 10");
}

int main(int argc, char** argv)
{
  if (argc < 5) {
    print_help();
    return -1;
  }

  int n = atoi(argv[1]);
  g_forks = calloc(n, sizeof(Fork));

  for (int i = 0; i < n; i++) {
    sem_init(&g_forks[i].lock, 0, 1);
  }

  Philosopher *philosophers = calloc(n, sizeof(Philosopher));

  for (int i = 0; i < n; i++) {
    philosophers[i].number = i + 1;
    philosophers[i].thinking_duration = exponential_random(atoi(argv[2]));
    philosophers[i].dining_duration = exponential_random(atoi(argv[3]));
    philosophers[i].max_cycle = atoi(argv[4]);
    philosophers[i].state = ST_THINKING;
    philosophers[i].left = &g_forks[i];
    philosophers[i].right = &g_forks[(i + 1) % n];
    pthread_create(&philosophers[i].tid, NULL, &run, &philosophers[i]);
  }

  for (int i = 0; i < n; i++) {
    pthread_join(philosophers[i].tid, NULL);
  }

  double total_hunger = 0;
  for (int i = 0; i < n; i++) {
    total_hunger += philosophers[i].measured_hunger_duration;
  }

  double average = total_hunger / n;
  double total_mean_sub = 0;

  for (int i = 0; i < n; i++) {
    total_mean_sub +=
        pow(fabs(philosophers[i].measured_hunger_duration - average), 2);
  }

  double std_deviation = sqrt(total_mean_sub / n);

  printf("Average hunger duration : %f sec\n", average);
  printf("Std deviation : %f\n", std_deviation);

  for (int i = 0; i < n; i++) {
    sem_destroy(&g_forks[i].lock);
  }

  free(philosophers);
  free(g_forks);

  return 0;
}
