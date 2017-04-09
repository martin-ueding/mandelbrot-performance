// Copyright © 2017 Martin Ueding <dev@martin-ueding.de>

#include "common.hpp"

#include <cmath>
#include <iostream>

void print_timings(double const time1,
                   double const time2,
                   size_t const sites) {
    std::cout << "Iterations: " << (time2 - time1) << "\t"
              << ((time2 - time1) / sites) << "\n";
}

uint8_t transform(int x, int max) {
    return std::log(x) * 255 / std::log(max);
}
