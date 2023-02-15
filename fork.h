#ifndef FORK_H_
#define FORK_H_
#include <semaphore.h>
typedef struct _Fork {
  sem_t lock;
} Fork;
void fork_init(Fork** fork);
void fork_close(Fork* fork);
#endif
