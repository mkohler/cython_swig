from nose.tools import eq_, ok_, raises

import cy_adder

# Test add

def test_add():
    eq_(cy_adder.add(2, 3), 5)

@raises(OverflowError)
def test_add_bignum():
    cy_adder.add(10000000000000000000000, 1)

@raises(TypeError)
def test_add_str():
    cy_adder.add('1', 1)


# Test add_sr

def test_add_sr():
    eq_(cy_adder.add_sr(2, 3), 5)

@raises(ArithmeticError)
def test_add_negative_number():
    cy_adder.add_sr(2, -2)


# Test get_version

def test_get_version():
    eq_(cy_adder.get_version(), cy_adder.ADDER_VERSION)

@raises(TypeError)
def test_get_version_extra_arg():
    cy_adder.get_version(1)

# Test get_version_sr

def test_get_version_sr():
    eq_(cy_adder.get_version(), cy_adder.ADDER_VERSION)


# Test make_greeting
def test_make_greeting():
    eq_(cy_adder.make_greeting("Python"),
        "Hello, Python")


# Test make_greeting_sr

def test_make_greeting_sr():
    eq_(cy_adder.make_greeting_sr("Python"),
        "Hello, Python")

@raises(MemoryError)
def test_make_greeting_sr_long_name():
    cy_adder.make_greeting_sr( "Python Programming Language")

if __name__ == '__main__':
    import nose
    nose.runmodule()
