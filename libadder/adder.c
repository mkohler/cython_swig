#include <stdlib.h>
#include <string.h>
#include "adder.h"

int
add(int x, int y) {
    return x + y;
}


char *
get_version(void) {
    return "v1.0";
}


#define MAX_STR_LEN 20

char *
make_greeting(char *name) {
    char * out_s = (char *)calloc(MAX_STR_LEN, 1);
    strcat(out_s, "Hello, ");
    strcat(out_s, name);
    return out_s;
}




/*
int
add_rv_is_status(int x, int y, int * sum) {
    *sum = x + y;
}


int
get_version_2(char *) {

}


int
put_a_string_in_my_buffer(char *str) {


}

*/
