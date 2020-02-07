// Copyright © 2017-2018 Martin Ueding <martin-ueding.de>

#pragma once

#include <cstddef>
#include <cstdint>

// http://stackoverflow.com/a/40765708/653152
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#define likely(expr) __builtin_expect(!!(expr), 1)

uint8_t transform(int x, int max, int to = 255);
double transform_double(double x, double max);

void print_timings(double const time1,
                   double const time2,
                   size_t const sites);
