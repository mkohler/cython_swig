cimport c_adder

def add(x, y):
    return c_adder.add(x, y)

def get_version():
    return c_adder.get_version()
