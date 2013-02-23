#!/bin/sh
set -e
set -x
. ./set_env.sh

# Build and test the C library.
autoreconf -i
mkdir -p build install
(cd build &&
    ../configure &&
    make &&
    make check &&
    make DESTDIR=$PWD/../install install)

# Build and test the SWIG extension.
(cd swig &&
    python setup.py build_ext --inplace &&
    python setup.py nosetests)

# Build and test the Cython extension.
(cd cython &&
    python setup.py build_ext --inplace &&
    python setup.py nosetests)


