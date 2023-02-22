#include "common.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>
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
