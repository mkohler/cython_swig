#!/usr/bin/python
import os
import sys
import adder
from nose.tools import eq_, raises

ADDER_VERSION = "v1.0"

# Test adder.add

def test_add():
    eq_(adder.add(2, 3), 5)

@raises(TypeError)
def test_add_str():
    eq_(adder.add(2, '2'), 9)


# Test adder.add_rs

def test_add_rs():
    eq_(adder.add_rs(2, 3), [0, 5])

@raises(TypeError)
def test_add_rs_str():
    eq_(adder.add_rs('1', 1)[0], 1)

def test_add_rs_negative_number():
    eq_(adder.add_rs(2, -2), [1, 0])


# Test adder.get_version

def test_get_version():
    eq_(adder.get_version(), ADDER_VERSION)

@raises(TypeError)
def test_get_version_extra_arg():
    adder.get_version('foo')


# Test adder.get_version_rs

def test_get_version_rs_small_buf():
    eq_(adder.get_version_rs(2), [1, ''])

def test_get_version_rs():
    eq_(adder.get_version_rs(64), [0, ADDER_VERSION])


# Test adder.greeting

def test_greeting():
    eq_(adder.greeting("Python"), "Hello, Python")


def test_greeting_rs_small_buf():
    eq_(adder.greeting_rs("Python", 5), [1, ''])

def test_greeting_rs():
    eq_(adder.greeting_rs("Python", 64), [0, "Hello, Python"])


# Test PAIR struct.

def test_pair():
    pair = adder.PAIR()
    pair.x = 3
    pair.y = 4
    eq_(adder.pair_add(pair), 7)


if __name__ == '__main__':
    import nose
    nose.runmodule()
