#include <stdlib.h>
#include <string.h>
#include "adder.h"

#define MAX_STR_LEN 20

static char version[] = "v1.0";
static char hello[] = "Hello, ";

int
add(int x, int y) {
    return x + y;
}

char *
get_version(void) {
    return version;
}


char *
greeting(char *name) {
    char * out_s = (char *)calloc(MAX_STR_LEN, 1);
    strcat(out_s, "Hello, ");
    strcat(out_s, name);
    return out_s;
}


int
add_rs(int x, int y, int *sum) {
    if ((x < 0) || (y < 0)) {
        *sum = 0;
        return 1;
    }
    *sum = x + y;
    return 0;
}


int
get_version_rs(char * output, int buflen) {
    if (buflen <= strlen(version) + 1) {
        output[0] = 0;
        return 1;
    }

    strcpy(output, version);
    return 0;
}


int
greeting_rs(char * name, char * outp, int buflen) {
    if (buflen < (strlen(hello) + strlen(name) + 1)) {
        outp[0] = 0;
        return 1;
    }
    strcpy(outp, hello);
    strcat(outp, name);
    return 0;
}

int
pair_add(PAIR * ppair) {
    return ppair->x + ppair->y;
}
