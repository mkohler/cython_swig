cimport c_adder

ADDER_VERSION = c_adder.ADDER_VERSION

def add(x, y):
    return c_adder.add(x, y)

def get_version():
    return c_adder.get_version()
