#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2017 Martin Ueding <dev@martin-ueding.de>

import argparse

import matplotlib.pyplot as pl
import numpy as np
import scipy.misc
import scipy.optimize as op


def dandify_axes(ax, legend=False):
    ax.grid(True)
    ax.margins(0.05)
    if legend:
        ax.legend(loc='best')


def dandify_figure(fig):
    fig.tight_layout()


def main():
    options = _parse_args()

    data = np.loadtxt(options.input)
    data = np.log(data + 1)
    scipy.misc.imsave(options.output, data)



def _parse_args():
    '''
    Parses the command line arguments.

    :return: Namespace with arguments.
    :rtype: Namespace
    '''
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('input')
    parser.add_argument('output')
    options = parser.parse_args()

    return options


if __name__ == '__main__':
    main()
