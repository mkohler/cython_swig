%module adder
%{
#include "adder.h"
%}

#define ADDER_VERSION "v1.0"

int add(int, int);
char * get_version(void);
char * make_greeting(char *name);

%include "typemaps.i"
/* Calling the parameter OUTPUT makes it so. */
int add_sr(int x, int y, int *OUTPUT);

%include "cstring.i"
%cstring_output_maxsize(char * out_str,  int buflen);
int get_version_sr(char * out_str, int buflen);

int make_greeting_sr(char * name, char * out_str, int buflen);
