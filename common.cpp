// Copyright © 2017-2018 Martin Ueding <dev@martin-ueding.de>

#include "common.hpp"

#include <cmath>
#include <iostream>

void print_timings(double const time1,
                   double const time2,
                   size_t const sites) {
    std::cout << "Iterations: " << (time2 - time1) << "\t"
              << ((time2 - time1) / sites) << "\n";
}

uint8_t transform(int x, int max, int to) {
    return std::log(x) * to / std::log(max);
}


double transform_double(double x, double max) {
    return std::log(x) * std::log(max);
}
