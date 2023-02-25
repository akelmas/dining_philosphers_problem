#include "statistics.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int calculate_bin_index(double src_value,
                               histogram_attributes_t* histogram_attributes) {
  return (src_value - histogram_attributes->min_value) /
         histogram_attributes->max_value * histogram_attributes->bin_count;
}

void statistics_analyze(statistics_t* statistics,
                        statistics_attributes_t* statistics_attributes,
                        double* dataptr, int len) {
  if (statistics_attributes->has_average) {
    statistics->average = 0;
    statistics->min_value = dataptr[0];
    statistics->max_value = dataptr[0];
    for (int i = 0; i < len; i++) {
      statistics->average += dataptr[i] / len;
      if (dataptr[i] > statistics->max_value) {
        statistics->max_value = dataptr[i];
      } else if (dataptr[i] < statistics->min_value) {
        statistics->min_value = dataptr[i];
      }
    }
    if (statistics_attributes->has_std_deviation) {
      double total_mean_sub = 0;
      for (int i = 0; i < len; i++) {
        total_mean_sub += pow(fabs(dataptr[i] - statistics->average), 2);
      }
      statistics->std_deviation = sqrt(total_mean_sub / len);
    }
  }
}

void statistics_create_histogram(histogram_t* histogram,
                                 histogram_attributes_t* histogram_attributes,
                                 double* dataptr, int len) {
  histogram->bins = (histogram_bin_t*)calloc(histogram_attributes->bin_count,
                                             sizeof(histogram_bin_t));
  histogram->attributes = *histogram_attributes;
  for (int i = 0; i < len; i++) {
    int bin_index = calculate_bin_index(dataptr[i], histogram_attributes);
    if (bin_index != -1) {
      histogram->bins[bin_index].frequency++;
      if (histogram->bins[bin_index].frequency > histogram->max_frequency) {
        histogram->max_frequency = histogram->bins[bin_index].frequency;
      }
    }
  }
}
