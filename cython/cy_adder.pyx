cimport c_adder

ADDER_VERSION = c_adder.ADDER_VERSION
def add(x, y):
    return c_adder.add(x, y)

def get_version():
    return c_adder.get_version()

def make_greeting(name):
    return c_adder.make_greeting(name)

def add_sr(x, y):
    cdef int sum_
    sr = c_adder.add_sr(x, y, &sum_)
    if sr:
        raise ArithmeticError
    return sum_

#def get_version_sr
