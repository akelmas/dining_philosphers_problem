#ifndef PHILOSOPHER_H_
#define PHILOSOPHER_H_
#include <pthread.h>

#include "fork.h"
enum State {
  ST_THINKING,
  ST_HUNGRY,
  ST_EATING,
  ST_FULL,
};
typedef struct _Philosopher {
  pthread_t tid;
  int index;
  int max_cycle;
  enum State state;
  int dining_duration;
  int thinking_duration;
  double measured_hunger_duration;
  Fork* left;
  Fork* right;
} Philosopher;
typedef void (*ActionCallback)(Philosopher*);
void philosopher_init(Philosopher** philosopher, int index,
                      int thinking_duration, int dining_duration,
                      int cycle_count, Fork* left, Fork* right);
void philosopher_start(Philosopher* philosopher);
void philosopher_finalize(Philosopher* philosopher);
void philosopher_close(Philosopher* philosopher);
#endif /* PHILOSOPHER_H_ */
