// Copyright © 2017-2018 Martin Ueding <dev@martin-ueding.de>

#pragma once

#include <cstddef>

double constexpr re_min = -2.4;
double constexpr re_max = 1.5;
double constexpr im_min = -1.3;
double constexpr im_max = 1.3;

size_t constexpr ly = 60 / 2.54 * 300;
//size_t constexpr ly = 1000;

double constexpr d = (im_max - im_min) / ly;
size_t constexpr lx = (re_max - re_min) / d;

size_t constexpr sites = lx * ly;

int constexpr max_iter = 300;
double constexpr radius = 5;

size_t constexpr veclen = 4;
size_t constexpr blocks_x = lx / veclen;
