cdef extern from "adder.h":
    int add(int x, int y)

    cdef char * get_version()
