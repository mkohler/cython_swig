#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "adder.h"
#include "minunit.h"

#define STR_BUF_LEN 32

MU_TEST(test_add) {
    mu_check(add(2,3) == 5);
}

MU_TEST(test_get_version) {
    mu_check_str_eq(get_version(), ADDER_VERSION);
}

MU_TEST(test_make_greeting) {
    mu_check_str_eq(make_greeting("C program"), "Hello, C program");
}

MU_TEST(test_add_sr) {
    int sum;
    int rv;

    rv = add_sr(2, 3, &sum);
    mu_check(rv == 0);
    mu_check(sum == 5);

    rv = add_sr(2, -3, &sum);
    mu_check(rv != 0);
}

MU_TEST(test_get_version_sr) {
    char output_s[STR_BUF_LEN];
    int rv;

    memset(output_s, 'x', STR_BUF_LEN);
    rv = get_version_sr(output_s, 2);
    mu_check(rv != 0);

    memset(output_s, 'x', STR_BUF_LEN);
    rv = get_version_sr(output_s, STR_BUF_LEN);
    mu_check(rv == 0);
    mu_check_str_eq(output_s, ADDER_VERSION);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_add);
    MU_RUN_TEST(test_get_version);
    MU_RUN_TEST(test_make_greeting);

    MU_RUN_TEST(test_add_sr);
    MU_RUN_TEST(test_get_version_sr);
}

int
main(void) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}

