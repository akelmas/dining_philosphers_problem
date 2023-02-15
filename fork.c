#include "fork.h"

#include <stdlib.h>
void fork_init(Fork** fork) {
  *fork = (Fork*)malloc(sizeof(Fork));
  sem_init(&(*fork)->lock, 0, 1);
}
void fork_close(Fork* fork) {
  if (fork != NULL) {
    sem_destroy(&fork->lock);
    free(fork);
  }
}
