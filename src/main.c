#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fork.h"
#include "philosopher.h"
void print_help(void) {
  printf(
      "\n%s\n\n"     // usage
      "%20s : %s\n"  // n
      "%20s : %s\n"  // thinking_duration
      "%20s : %s\n"  // dining_duration
      "%20s : %s\n"  // cycle
      "\n%s\n",      // example
      "Usage : ./dining_philosophers n thinking_duration dining_duration "
      "cycle",
      "n", "number of philosophers to emulate", "thinking_duration",
      "expected time (us) for thinking", "dining_duration",
      "expected time (us) for dining", "cycle",
      "count to repeat cycle: THINKING->HUNGER->EATING",
      "Example : ./dining_philosophers 5 210 20 10");
}
int main(int argc, char** argv) {
  if (argc < 5) {
    print_help();
    return -1;
  }
  int number = atoi(argv[1]);
  int thinking_duration = atoi(argv[2]);
  int dining_duration = atoi(argv[3]);
  int max_cycle = atoi(argv[4]);
  philosopher_t* philosophers[number];
  fork_t* forks[number];
  for (int i = 0; i < number; i++) {
    fork_init(&forks[i]);
  }
  for (int i = 0; i < number; i++) {
    philosopher_init(&philosophers[i], i, thinking_duration, dining_duration,
                     max_cycle, forks[i], forks[(i + 1) % number]);
  }
  for (int i = 0; i < number; i++) {
    philosopher_start(philosophers[i]);
  }
  for (int i = 0; i < number; i++) {
    philosopher_finalize(philosophers[i]);
  }
  double total_hunger = 0;
  for (int i = 0; i < number; i++) {
    total_hunger += philosophers[i]->measured_hunger_duration;
  }
  double average = total_hunger / number;
  double total_mean_sub = 0;
  for (int i = 0; i < number; i++) {
    total_mean_sub +=
        pow(fabs(philosophers[i]->measured_hunger_duration - average), 2);
  }
  double std_deviation = sqrt(total_mean_sub / number);
  printf("Average hunger duration : %f sec\n", average);
  printf("Std deviation : %f\n", std_deviation);
  for (int i = 0; i < number; i++) {
    philosopher_close(philosophers[i]);
  }
  for (int i = 0; i < number; i++) {
    fork_close(forks[i]);
  }
  return 0;
}
