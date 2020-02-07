// Copyright © 2017-2018 Martin Ueding <martin-ueding.de>

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
  std::cout << "Computing a " << lx << " × " << ly << " pixel image …"
            << std::endl;
  std::vector<int> escape_iter(lx * ly, max_iter);

  auto const time1 = omp_get_wtime();

#pragma omp parallel for collapse(2)
    for (size_t y = 0; y < ly; ++y) {
        for (size_t x = 0; x < lx; ++x) {
            double z_re = 0.0;
            double z_im = 0.0;
            for (size_t iter = 0; iter < max_iter; ++iter) {
                auto const new_z_re = z_re * z_re - z_im * z_im + c_re(x);
                auto const new_z_im = 2 * z_re * z_im + c_im(y);

                auto const new_abs_sq = new_z_re * new_z_re + new_z_im * new_z_im;

                if (unlikely(new_abs_sq >= radius)) {
                  auto const abs = std::sqrt(z_re * z_re + z_im * z_im);

                  escape_iter[idx(x, y)] = iter;
                  break;
                }

                z_re = new_z_re;
                z_im = new_z_im;

            }
        }
    }

    auto const normalization_escape =
        *std::max_element(std::begin(escape_iter), std::end(escape_iter));

    auto const time2 = omp_get_wtime();

    cv::Mat lattice(ly, lx, CV_32FC3);
    cv::Mat lattice_rgb(ly, lx, CV_32FC3);
    for (size_t y = 0; y < ly; ++y) {
        for (size_t x = 0; x < lx; ++x) {
          cv::Vec3f in(0, 0, 0);

            auto const es = transform_double(
                std::max(4, escape_iter[idx(x, y)]), normalization_escape);

            if (escape_iter[idx(x, y)] < normalization_escape) {
              in = cv::Vec3f(10 + 10 * es, 1.0 - es, es);
          }

          lattice.at<cv::Vec3f>(y, x) = in;

          //cv::line(lattice, cv::Point(x, y), cv::Point(x, y), in);
        }
    }
    cv::imwrite("mandelbrot-cpp.png", lattice);

    cv::cvtColor(lattice, lattice_rgb, cv::COLOR_HSV2BGR, 3);
    cv::imwrite("mandelbrot-cpp2.png", lattice_rgb);


    print_timings(time1, time2, sites);
}
