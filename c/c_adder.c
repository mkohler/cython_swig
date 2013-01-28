#include <stdio.h>
#include "adder.h"

int
main(void) {
    if (add(2, 3) == 5) {
        return 0;
    }
    return 1;
}
