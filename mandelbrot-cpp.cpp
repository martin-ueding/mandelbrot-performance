// Copyright © 2017 Martin Ueding <dev@martin-ueding.de>

#include "common.hpp"
#include "param.hpp"

#include <omp.h>
#include <opencv2/opencv.hpp>

#include <vector>

size_t idx(size_t const x, size_t const y) {
    return y * lx + x;
}

size_t idx(size_t const x, size_t const y, size_t const z) {
    return (y * lx + x) * 2 + z;
}

double c_re(size_t const x) {
    return re_min + x * d;
}

double c_im(size_t const y) {
    return im_min + y * d;
}

int main(int argc, char **argv) {
    std::vector<uint8_t> escape_iter(lx * ly, max_iter);

    auto const time1 = omp_get_wtime();

#pragma omp parallel for collapse(2)
    for (size_t y = 0; y < ly; ++y) {
        for (size_t x = 0; x < lx; ++x) {
            double z_re = 0.0;
            double z_im = 0.0;
            for (size_t iter = 0; iter < max_iter; ++iter) {
                auto const abs = z_re * z_re + z_im * z_im;

                if (unlikely(abs >= radius)) {
                    escape_iter[idx(x, y)] = iter;
                    break;
                }

                auto const new_z_re = z_re * z_re - z_im * z_im + c_re(x);
                auto const new_z_im = 2 * z_re * z_im + c_im(y);

                z_re = new_z_re;
                z_im = new_z_im;
            }
        }
    }

    auto const time2 = omp_get_wtime();


    cv::Mat lattice(ly, lx, CV_8UC1);
    for (size_t y = 0; y < ly; ++y) {
        for (size_t x = 0; x < lx; ++x) {
            lattice.at<uint8_t>(y, x) =
                transform(escape_iter[idx(x, y)], max_iter);
        }
    }
    cv::imwrite("mandelbrot-cpp.png", lattice);


    print_timings(time1, time2, sites);
}
