# Copyright © 2017 Martin Ueding <dev@martin-ueding.de>

cmake_minimum_required(VERSION 3.0)

project(mandelbrot-performance C CXX)

add_definitions(-Wall -Wpedantic --std=c++11)

find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

find_package(OpenCV REQUIRED)

add_executable(mandelbrot-cpp
    mandelbrot-cpp.cpp

    common.cpp
    )
add_executable(mandelbrot-soa
    mandelbrot-soa.cpp

    common.cpp
    )

target_link_libraries(mandelbrot-cpp ${OpenCV_LIBS} ${OpenMP_LIBS})
target_link_libraries(mandelbrot-soa ${OpenCV_LIBS} ${OpenMP_LIBS})
