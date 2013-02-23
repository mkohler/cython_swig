#include <stdlib.h>
#include <string.h>
#include "adder.h"

#define MAX_STR_LEN 20

int
add(int x, int y) {
    return x + y;
}



char *
get_version(void) {
    return ADDER_VERSION;
}


char *
make_greeting(char *name) {
    char * out_s = (char *)calloc(MAX_STR_LEN, 1);
    strcat(out_s, "Hello, ");
    strcat(out_s, name);
    return out_s;
}


int
add_sr(int x, int y, int *sum) {
    if ((x < 0) || (y < 0)) {
        *sum = 0;
        return 1;
    }
    *sum = x + y;
    return 0;
}


int
get_version_sr(char * output, int buflen) {
    if (buflen <= strlen(ADDER_VERSION) + 1) {
        output[0] = 0;
        return 1;
    }

    strcpy(output, ADDER_VERSION);
    return 0;
}


#define GREETING "Hello, "
int
make_greeting_sr(char * name, char * output, int buflen) {
    if (buflen < (strlen(GREETING) + strlen(name) + 1)) {
        output[0] = 0;
        return 1;
    }
    strcpy(output, GREETING);
    strcat(output, name);
    return 0;
}
