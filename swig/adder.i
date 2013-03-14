%module adder
%{
#include "adder.h"
%}

typedef struct _PAIR {
    int x;
    int y;
} PAIR;

int add(int, int);
char * get_version(void);
char * greeting(char *name);

%include "typemaps.i"
/* Calling the parameter OUTPUT makes it so. */
int add_rs(int x, int y, int *OUTPUT);

%include "cstring.i"
%cstring_output_maxsize(char * outp,  int buflen);
int get_version_rs(char * outp, int buflen);

int sgreeting(char * name, char * outp, int buflen);

int pair_add(PAIR *);
