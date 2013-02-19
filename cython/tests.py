from nose.tools import eq_, ok_

import cy_adder

def test_add():
    eq_(cy_adder.add(2, 3), 5)

def test_get_version():
    eq_(cy_adder.get_version(), cy_adder.ADDER_VERSION)
