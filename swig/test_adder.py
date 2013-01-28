#!/usr/bin/python
import adder
from nose.tools import eq_


def test_adder():
    eq_(adder.add(2, 3), 5)

if __name__ == '__main__':
    import nose
    nose.runmodule()
