#include "histogram.h"

void find_minmax(const vector<long double>& numbers, long double &min, long double &max) {
      for (double number : numbers) {
          if (number < min) {
              min = number;
          }
          if (number > max) {
              max = number;
          }
      }
  }
