cdef extern from "adder.h":

    char * ADDER_VERSION = "v1.0"

    int add(int x, int y)

    cdef char * get_version()
