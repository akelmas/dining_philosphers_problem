#include "philosopher.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "fork.h"
static void think(philosopher_t* philosopher) {
  usleep(philosopher->thinking_duration);
  philosopher->state = ST_HUNGRY;
}
static void take_forks(philosopher_t* philosopher) {
  sem_wait(&philosopher->left->lock);
  sem_wait(&philosopher->right->lock);
  philosopher->state = ST_EATING;
}
static void eat(philosopher_t* philosopher) {
  usleep(philosopher->dining_duration);
  philosopher->state = ST_FULL;
}
static void release_forks(philosopher_t* philosopher) {
  sem_post(&philosopher->left->lock);
  sem_post(&philosopher->right->lock);
  philosopher->max_cycle--;
  philosopher->state = ST_THINKING;
}
static void perform(philosopher_t* philosopher, ActionCallback action_callback,
                    double* duration_us) {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  action_callback(philosopher);
  clock_gettime(CLOCK_MONOTONIC, &end);
  *duration_us = (end.tv_sec - start.tv_sec);
  *duration_us += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}
static void* run(void* arg) {
  philosopher_t* philosopher = arg;
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
void philosopher_init(philosopher_t** philosopher, int index,
                      int thinking_duration, int dining_duration,
                      int cycle_count, fork_t* left, fork_t* right) {
  (*philosopher) = (philosopher_t*)malloc(sizeof(philosopher_t));
  (*philosopher)->index = index;
  (*philosopher)->thinking_duration = exponential_random(thinking_duration);
  (*philosopher)->dining_duration = exponential_random(dining_duration);
  (*philosopher)->max_cycle = cycle_count;
  (*philosopher)->state = ST_THINKING;
  (*philosopher)->left = left;
  (*philosopher)->right = right;
  (*philosopher)->tid = 0;
}
void philosopher_start(philosopher_t* philosopher) {
  if (philosopher != NULL) {
    pthread_create(&philosopher->tid, NULL, &run, philosopher);
  }
}
void philosopher_finalize(philosopher_t* philosopher) {
  if (philosopher != NULL) {
    pthread_join(philosopher->tid, NULL);
  }
}
void philosopher_close(philosopher_t* philosopher) {
  if (philosopher != NULL) {
    free(philosopher);
  }
}
