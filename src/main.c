#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fork.h"
#include "philosopher.h"
#include "statistics.h"

#define MAX_WIDTH 80
#define MAX_HEIGHT 32
#define INTERVAL_PLACEHOLDER_LENGTH 12
#define BIN_OFFSET INTERVAL_PLACEHOLDER_LENGTH + 1

void print_bin(int src_frequency, int max_frequency) {
  double bin_length =
      (double)(MAX_WIDTH - BIN_OFFSET) * src_frequency / max_frequency;
  putchar('|');
  for (int i = 0; i < bin_length; i++) {
    printf("\u2588");
  }
  putchar('\n');
}

void print_horizontal_axis_label(const char* label) {
  int label_offset = (BIN_OFFSET + MAX_WIDTH + strlen(label)) / 2;
  printf("%*s\n", label_offset, label);
}

void print_horizontal_axis(int max_frequency) {
  for (int i = 0; i < BIN_OFFSET; i++) putchar(' ');
  putchar('0');
  for (int i = 0; i < MAX_WIDTH - BIN_OFFSET - 1; i++) putchar('_');
  printf("%d", max_frequency);
  putchar('\n');
}

void print_histogram(histogram_t* histogram) {
  double range =
      (histogram->attributes.max_value - histogram->attributes.min_value) /
      histogram->attributes.bin_count;
  print_horizontal_axis_label("Frequency");
  print_horizontal_axis(histogram->max_frequency);
  for (int i = 0; i < histogram->attributes.bin_count; i++) {
    printf("%5.f - %-5.f", range * i * 1000000, range * (i + 1) * 1000000);
    print_bin(histogram->bins[i].frequency, histogram->max_frequency);
  }
}

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
  double hunger_durations[number];
  statistics_attributes_t statistics_attributes = {.has_average = 1,
                                                   .has_std_deviation = 1};
  statistics_t statistics = {0};
  histogram_attributes_t histogram_attributes;
  histogram_t histogram = {0};

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
    hunger_durations[i] = philosophers[i]->measured_hunger_duration;
  }
  statistics_analyze(&statistics, &statistics_attributes, hunger_durations,
                     number);
  printf("Average hunger duration : %f sec\n", statistics.average);
  printf("Std deviation : %f\n", statistics.std_deviation);
  histogram_attributes.bin_count = MAX_HEIGHT;
  histogram_attributes.max_value = statistics.max_value;
  histogram_attributes.min_value = statistics.min_value;
  statistics_create_histogram(&histogram, &histogram_attributes,
                              hunger_durations, number);

  print_histogram(&histogram);

  for (int i = 0; i < number; i++) {
    philosopher_close(philosophers[i]);
  }
  for (int i = 0; i < number; i++) {
    fork_close(forks[i]);
  }
  return 0;
}
