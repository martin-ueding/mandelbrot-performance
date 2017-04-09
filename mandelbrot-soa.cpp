// Copyright © 2017 Martin Ueding <dev@martin-ueding.de>

#include "common.hpp"
#include "param.hpp"

#include <omp.h>
#include <opencv2/opencv.hpp>

#include <memory>
#include <vector>

size_t idx(size_t const bx, size_t const y) {
    return y * blocks_x + bx;
}

double c_re(size_t const bx, size_t const x) {
    return re_min + (bx * veclen + x) * d;
}

double c_im(size_t const y) {
    return im_min + y * d;
}

struct soa {
    alignas(veclen * sizeof(double)) double z[2][veclen];
    // alignas(veclen * sizeof(int)) int z[veclen] = {0};
};

int main(int argc, char **argv) {
    std::vector<size_t> escape_iter(lx * ly, max_iter);

    auto const time1 = omp_get_wtime();

#pragma omp parallel for collapse(2)
    for (size_t y = 0; y < ly; ++y) {
        for (size_t bx = 0; bx < blocks_x; ++bx) {
            bool finished[veclen] = {false};
            double re[veclen] = {0.0};
            double im[veclen] = {0.0};
            double re[veclen] = {0.0};
            double im[veclen] = {0.0};
            for (size_t iter = 0; iter < max_iter; ++iter) {
#pragma omp simd aligned(z : 256)
                for (size_t v = 0; v < veclen; ++v) {
                    auto const z_re = z[0][v];
                    auto const z_im = z[1][v];

                    auto const abs = z_re * z_re + z_im * z_im;

                    z[0][v] = z_re * z_re - z_im * z_im + c_re(bx, v);
                    z[1][v] = 2 * z_re * z_im + c_im(y);

                    if (unlikely(abs > radius)) {
                        finished[v] = true;
                    }
                }

                bool all_finished = true;
                for (size_t v = 0; v < veclen; ++v) {
                    if (finished[v] &&
                        escape_iter[idx(bx, y) * veclen + v] == max_iter) {
                        escape_iter[idx(bx, y) * veclen + v] = iter;
                    }
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
