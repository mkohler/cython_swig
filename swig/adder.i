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
int add_sr(int x, int y, int *OUTPUT);

%include "cstring.i"
%cstring_output_maxsize(char * outp,  int buflen);
int get_version_sr(char * outp, int buflen);

int greeting_sr(char * name, char * outp, int buflen);

int pair_add(PAIR *);
