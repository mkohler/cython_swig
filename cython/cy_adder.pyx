cimport c_adder

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
    # We know it can't fail, so we ignore the return value.
    c_adder.get_version_sr(version_str, BUF_LEN)
    return version_str


def greeting(name):
    return c_adder.greeting(name)

def greeting_sr(name):
    DEF BUF_LEN = 16
    cdef char out_str[BUF_LEN]
    if len(name) + 10 > BUF_LEN:
        raise MemoryError
    c_adder.greeting_sr(name, out_str, BUF_LEN)
    return out_str



