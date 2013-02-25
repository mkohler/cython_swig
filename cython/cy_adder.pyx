cimport c_adder

ADDER_VERSION = c_adder.ADDER_VERSION

def add(x, y):
    return c_adder.add(x, y)

def add_sr(x, y):
    cdef int sum_
    sr = c_adder.add_sr(x, y, &sum_)
    if sr:
        raise ArithmeticError
    return sum_


def get_version():
    return c_adder.get_version()

def get_version_sr():
    DEF BUF_LEN = 32
    cdef char version_str[BUF_LEN]
    sr = c_adder.get_version_sr(version_str, BUF_LEN)
    # TODO


def make_greeting(name):
    return c_adder.make_greeting(name)


#def get_version_sr
