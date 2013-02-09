#include <stdio.h>
#include "adder.h"

int
main(void) {
    printf("add(2,3) = %d\n", add(2, 3));
    puts(get_version());
    puts(make_greeting("C program"));
    return 0;
}
