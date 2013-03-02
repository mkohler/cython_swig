cdef extern from "adder.h":

    char * ADDER_VERSION = "v1.0"
    int add(int x, int y)
    char * get_version()
    char * greeting(char * name)

    int add_sr(int x, int y, int *sum)
    int get_version_sr(char * output, int buflen)
    int greeting_sr(char * name, char * output, int buflen)
