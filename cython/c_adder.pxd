cdef extern from "adder.h":

    ctypedef struct PAIR:
        int x
        int y

    int add(int x, int y)
    char * get_version()
    char * greeting(char * name)

    int add_rs(int x, int y, int *sum)
    int get_version_rs(char * output, int buflen)
    int greeting_rs(char * name, char * output, int buflen)

    int pair_add(PAIR * ppair)
