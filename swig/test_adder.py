#!/usr/bin/python
import os
import sys
import adder
from nose.tools import eq_


def test_add():
    eq_(adder.add(2, 3), 5)

def test_add_sr():
    eq_(adder.add_sr(2, 3), [0, 5])


def test_get_version():
    eq_(adder.get_version(), adder.ADDER_VERSION)

def test_get_version_sr_small_buf():
    eq_(adder.get_version_sr(2), [1, ''])

def test_get_version_sr():
    eq_(adder.get_version_sr(64), [0, adder.ADDER_VERSION])


def test_make_greeting():
    eq_(adder.make_greeting("Python"), "Hello, Python")

def test_make_greeting_sr_small_buf():
    eq_(adder.make_greeting_sr("Python", 5), [1, ""])

def test_make_greeting_sr():
    eq_(adder.make_greeting_sr("Python", 64), [0, "Hello, Python"])


if __name__ == '__main__':
    import nose
    nose.runmodule()
