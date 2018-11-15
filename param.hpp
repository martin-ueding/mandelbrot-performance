// Copyright © 2017 Martin Ueding <dev@martin-ueding.de>

#pragma once

#include <cstddef>

double constexpr re_min = -2.4;
double constexpr re_max = 1.5;
double constexpr im_min = -1.3;
double constexpr im_max = 1.3;

double constexpr d = (im_max - im_min) / (50.0 * 20.0);

size_t constexpr lx = (re_max - re_min) / d;
size_t constexpr ly = (im_max - im_min) / d;

size_t constexpr sites = lx * ly;

uint8_t constexpr max_iter = 100;
double constexpr radius = 5;

size_t constexpr veclen = 4;
size_t constexpr blocks_x = lx / veclen;
