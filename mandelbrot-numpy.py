#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2017 Martin Ueding <dev@martin-ueding.de>

import argparse
import time

import numpy as np


def main():
    options = _parse_args()

    re_min = -2.1
    re_max = 0.6
    im_min = -1.3
    im_max = 1.3

    d = 0.002

    lx = int((re_max - re_min) / d)
    ly = int((im_max - im_min) / d)

    sites = lx * ly

    im, re = np.mgrid[im_min:im_max:d, re_min:re_max:d]
    c = 1.0 * re + im * 1.0j
    z = np.zeros_like(c)

    radius = 5
    max_iter = 100

    iters = np.zeros_like(c,  dtype=int)

    time1 = time.time()

    for i in range(100):
        z = z**2 + c
        a = np.abs(z)
        selection = (a >= radius) & (iters == 0)
        sel = np.where(selection)
        iters[sel] = i
        if selection.all():
            break

    iters[np.where(iters == 0)] = i

    time2 = time.time()

    print('Iterations:', time2 - time1, (time2 - time1) / sites)

    np.savetxt('mandelbrot.txt', iters)


def _parse_args():
    '''
    Parses the command line arguments.

    :return: Namespace with arguments.
    :rtype: Namespace
    '''
    parser = argparse.ArgumentParser(description='')
    options = parser.parse_args()

    return options


if __name__ == '__main__':
    main()
