#ifndef FORK_H_
#define FORK_H_
#include <semaphore.h>
typedef struct _fork {
  sem_t lock;
} fork_t;
void fork_init(fork_t** fork);
void fork_close(fork_t* fork);
#endif
