#!/bin/sh
set -e
set -x
. ./set_env.sh

autoreconf -i
mkdir -p build install
(cd build &&
    ../configure &&
    make &&
    make check &&
    make DESTDIR=$PWD/../install install)

(cd cython &&
    python setup.py build_ext &&
    python setup.py nosetests)


