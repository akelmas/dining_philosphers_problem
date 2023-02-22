#include "fork.h"

#include <stdlib.h>
void fork_init(fork_t** fork) {
  *fork = (fork_t*)malloc(sizeof(fork_t));
  sem_init(&(*fork)->lock, 0, 1);
}
void fork_close(fork_t* fork) {
  if (fork != NULL) {
    sem_destroy(&fork->lock);
    free(fork);
  }
}
