#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "adder.h"
#include "minunit.h"

int tests_run = 0;


MU_TEST(test_add) {
    mu_check(add(2,3) == 5);
}

MU_TEST(test_get_version) {
    mu_check_str_eq(get_version(), ADDER_VERSION);
}
#if 0

MU_TEST(test_adder) {
    mu_check(make_greeting("C program"));
}


#define STR_BUF_LEN 32

void
test_adder_sr(void) {
    char output_s[STR_BUF_LEN];
    int sum;
    int rv;

    rv = add_sr(2, 3, &sum);
    printf("add_sr(2, 3) returned %d, sum = %d\n", rv, sum);
    rv = add_sr(2, -3, &sum);
    printf("add_sr(2, -3) returned %d, sum = %d\n", rv, sum);


    memset(output_s, 'x', STR_BUF_LEN);
    rv = get_version_sr(output_s, 2);
    printf("get_version_sr(output_s, 2) returned %d, output_s = %s\n",
           rv, output_s);

    memset(output_s, 'x', STR_BUF_LEN);
    rv = get_version_sr(output_s, STR_BUF_LEN);
    printf("get_version_sr(output_s, 64) returned %d, output_s = %s\n",
           rv, output_s);



}

#endif
MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_add);
    MU_RUN_TEST(test_get_version);
}

int
main(void) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}

