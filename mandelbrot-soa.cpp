// Copyright © 2017 Martin Ueding <martin-ueding.de>

#include "common.hpp"
#include "param.hpp"

#include <omp.h>
#include <opencv2/opencv.hpp>

#include <memory>
#include <vector>

size_t idx(size_t const bx, size_t const y) {
    return y * blocks_x + bx;
}

double get_c_re(size_t const bx, size_t const x) {
    return re_min + (bx * veclen + x) * d;
}

double get_c_im(size_t const y) {
    return im_min + y * d;
}

struct soa {
    alignas(veclen * sizeof(double)) double z[2][veclen];
    // alignas(veclen * sizeof(int)) int z[veclen] = {0};
};

int main(int argc, char **argv) {
    std::vector<uint8_t> escape_iter(lx * ly, max_iter);

    auto const time1 = omp_get_wtime();

#pragma omp parallel for collapse(2)
    for (size_t y = 0; y < ly; ++y) {
        for (size_t bx = 0; bx < blocks_x; ++bx) {
            alignas(veclen * sizeof(bool)) bool finished[veclen] = {false};
            alignas(veclen * sizeof(bool)) bool written[veclen] = {false};

            alignas(veclen * sizeof(double)) double z_re[veclen] = {0.0};
            alignas(veclen * sizeof(double)) double z_im[veclen] = {0.0};
            alignas(veclen * sizeof(double)) double new_z_re[veclen];
            alignas(veclen * sizeof(double)) double new_z_im[veclen];

            alignas(veclen * sizeof(double)) double c_re[veclen] = {0.0};
            for (size_t v = 0; v < veclen; ++v) {
                c_re[v] = get_c_re(bx, v);
            }
            double c_im = get_c_im(y);

            for (size_t iter = 0; iter < max_iter; ++iter) {
#pragma omp simd aligned(z_re, z_im, new_z_re, new_z_im, finished)
                for (size_t v = 0; v < veclen; ++v) {
                    new_z_re[v] =
                        z_re[v] * z_re[v] - z_im[v] * z_im[v] + c_re[v];
                    new_z_im[v] = 2 * z_re[v] * z_im[v] + c_im;

                    z_re[v] = new_z_re[v];
                    z_im[v] = new_z_im[v];

                    auto const abs = z_re[v] * z_re[v] + z_im[v] * z_im[v];

                    if (!finished[v] && unlikely(abs > radius)) {
                        finished[v] = true;
                        escape_iter[idx(bx, y) * veclen + v] = iter;
                    }
                }

                bool all_finished = true;
                for (size_t v = 0; v < veclen; ++v) {
                    all_finished &= finished[v];
                }

                if (all_finished) {
                    break;
                }
            }
        }
    }

    auto const time2 = omp_get_wtime();

    cv::Mat lattice(ly, lx, CV_8UC1);
    for (size_t y = 0; y < ly; ++y) {
        for (size_t bx = 0; bx < blocks_x; ++bx) {
            for (size_t v = 0; v < veclen; ++v) {
                lattice.at<uint8_t>(y, bx * veclen + v) =
                    transform(escape_iter[idx(bx, y) * veclen + v], max_iter);
            }
        }
    }
    cv::imwrite("mandelbrot-soa.png", lattice);

    print_timings(time1, time2, sites);
}
