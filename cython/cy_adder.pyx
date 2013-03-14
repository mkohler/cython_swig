cimport c_adder

def add(x, y):
    return c_adder.add(x, y)

def add_rs(x, y):
    cdef int sum_
    sr = c_adder.add_rs(x, y, &sum_)
    if sr:
        raise ArithmeticError
    return sum_


def get_version():
    return c_adder.get_version()

def get_version_rs():
    DEF BUF_LEN = 32
    cdef char version_str[BUF_LEN]
    # We know it can't fail, so we ignore the return value.
    c_adder.get_version_rs(version_str, BUF_LEN)
    return version_str


def greeting(name):
    return c_adder.greeting(name)

def sgreeting(name):
    c_str_len = c_adder.sgreeting(name, <char *>0, 0)
    py_str = ' ' * (c_str_len + 1)
    cdef char * c_str = py_str
    c_adder.sgreeting(name, c_str, len(py_str))
    return c_str


def pair_add(x, y):
    cdef c_adder.PAIR pair
    pair.x = x
    pair.y = y
    return c_adder.pair_add(&pair)
