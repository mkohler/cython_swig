#!/usr/bin/python
import os
import sys
print "working directory", os.getcwd()
print "sys.path:", sys.path
import adder
from nose.tools import eq_


def test_adder():
    eq_(adder.add(2, 3), 5)

if __name__ == '__main__':
    import nose
    nose.runmodule()
