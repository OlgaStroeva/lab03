#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include <vector>
#include <iostream>
using namespace std;

void find_minmax(const vector<long double>& numbers, long double &min, long double &max);

size_t ChooseYourColor(string color);

void Scale_under_image(const size_t max_count, size_t bin_count); //Code for task 15.

void draw_histogram_svg(const vector<size_t>& bins);

#endif // HISTOGRAM_H
