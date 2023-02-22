#ifndef PHILOSOPHER_H_
#define PHILOSOPHER_H_
#include <pthread.h>

#include "fork.h"
typedef enum _state {
  ST_THINKING,
  ST_HUNGRY,
  ST_EATING,
  ST_FULL,
} state_t;
typedef struct _philosopher {
  pthread_t tid;
  int index;
  int max_cycle;
  state_t state;
  int dining_duration;
  int thinking_duration;
  double measured_hunger_duration;
  fork_t* left;
  fork_t* right;
} philosopher_t;
typedef void (*ActionCallback)(philosopher_t*);
void philosopher_init(philosopher_t** philosopher, int index,
                      int thinking_duration, int dining_duration,
                      int cycle_count, fork_t* left, fork_t* right);
void philosopher_start(philosopher_t* philosopher);
void philosopher_finalize(philosopher_t* philosopher);
void philosopher_close(philosopher_t* philosopher);
#endif /* PHILOSOPHER_H_ */
