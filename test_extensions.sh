#!/bin/sh
set -e
set -x

make DESTDIR=$PWD/install install
. ./set_env.sh
(cd swig && python setup.py nosetests)
(cd cython && python setup.py nosetests)
