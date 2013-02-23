#!/bin/sh
set -e
set -x
. ./set_env.sh

# Build and test the C library.
autoreconf -i
./configure
mkdir -p install
make DESTDIR=$PWD/install check install

# Build and test the SWIG extension.
(cd swig &&
    python setup.py build_ext --inplace &&
    python setup.py nosetests)

# Build and test the Cython extension.
(cd cython &&
    python setup.py build_ext --inplace &&
    python setup.py nosetests)
