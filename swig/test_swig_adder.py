#!/usr/bin/python
import os
import sys
import adder
from nose.tools import eq_, raises

# Test adder.add

def test_add():
    eq_(adder.add(2, 3), 5)

@raises(TypeError)
def test_add_str():
    eq_(adder.add(2, '2'), 9)


# Test adder.add_sr

def test_add_sr():
    eq_(adder.add_sr(2, 3), [0, 5])

@raises(TypeError)
def test_add_sr_str():
    eq_(adder.add_sr('1', 1)[0], 1)

def test_add_sr_negative_number():
    eq_(adder.add_sr(2, -2), [1, 0])


# Test adder.get_version

def test_get_version():
    eq_(adder.get_version(), adder.ADDER_VERSION)

@raises(TypeError)
def test_get_version_extra_arg():
    adder.get_version('foo')


# Test adder.get_version_sr

def test_get_version_sr_small_buf():
    eq_(adder.get_version_sr(2), [1, ''])

def test_get_version_sr():
    eq_(adder.get_version_sr(64), [0, adder.ADDER_VERSION])


# Test adder.make_greeting

def test_make_greeting():
    eq_(adder.make_greeting("Python"), "Hello, Python")


def test_make_greeting_sr_small_buf():
    eq_(adder.make_greeting_sr("Python", 5), [1, ''])

def test_make_greeting_sr():
    eq_(adder.make_greeting_sr("Python", 64), [0, "Hello, Python"])


if __name__ == '__main__':
    import nose
    nose.runmodule()
