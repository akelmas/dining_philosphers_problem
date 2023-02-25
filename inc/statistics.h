#ifndef STATISTICS_H_
#define STATISTICS_H_

typedef struct _statistics_attributes {
  unsigned char has_average;
  unsigned char has_std_deviation;
} statistics_attributes_t;
typedef struct _statistics {
  double average;
  double std_deviation;
  double min_value;
  double max_value;
} statistics_t;
typedef struct _histrogram_bin {
  int frequency;
} histogram_bin_t;
typedef struct _histogram_attributes {
  double max_value;
  double min_value;
  int bin_count;
} histogram_attributes_t;
typedef struct _histogram {
  histogram_bin_t* bins;
  int max_frequency;
  histogram_attributes_t attributes;
} histogram_t;
void statistics_analyze(statistics_t* statistics,
                        statistics_attributes_t* statistics_attributes,
                        double* dataptr, int len);
void statistics_create_histogram(histogram_t* histogram,
                                 histogram_attributes_t* histogram_attributes,
                                 double* dataptr, int len);
#endif
