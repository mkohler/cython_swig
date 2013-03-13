#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "adder.h"
#include "minunit.h"

#define ADDER_VERSION "v1.0"
#define STR_BUF_LEN 32

MU_TEST(test_add) {
    mu_check(add(2,3) == 5);
}

MU_TEST(test_get_version) {
    mu_check_str_eq(get_version(), ADDER_VERSION);
}

MU_TEST(test_greeting) {
    mu_check_str_eq(greeting("C program"), "Hello, C program");
}

MU_TEST(test_add_rs) {
    int sum;
    int rv;

    rv = add_rs(2, 3, &sum);
    mu_check(rv == 0);
    mu_check(sum == 5);

    rv = add_rs(2, -3, &sum);
    mu_check(rv != 0);
}

MU_TEST(test_get_version_rs) {
    char output_s[STR_BUF_LEN];
    int rv;

    memset(output_s, 'x', STR_BUF_LEN);
    rv = get_version_rs(output_s, 2);
    mu_check(rv != 0);
    mu_check_str_eq(output_s, "");

    memset(output_s, 'x', STR_BUF_LEN);
    rv = get_version_rs(output_s, STR_BUF_LEN);
    mu_check(rv == 0);
    mu_check_str_eq(output_s, ADDER_VERSION);
}

MU_TEST(test_greeting_rs_null_pointer) {
    mu_check(greeting_rs("Python", 0, 0) == \
             strlen("Python") + strlen(ADDER_GREETING));
}

MU_TEST(test_greeting_rs) {
    char output_s[STR_BUF_LEN];
    int rv;

    memset(output_s, 'x', STR_BUF_LEN);
    rv = greeting_rs("Python", output_s, 5);
    mu_check(rv == 0);

    memset(output_s, 'x', STR_BUF_LEN);
    rv = greeting_rs("Python", output_s, STR_BUF_LEN);
    mu_check(rv == 13);
    mu_check_str_eq(output_s, "Hello, Python");
}


MU_TEST(test_add_struct) {
    PAIR pair = {3, 4};
    mu_check(pair_add(&pair) == 7);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_add);
    MU_RUN_TEST(test_get_version);
    MU_RUN_TEST(test_greeting);

    MU_RUN_TEST(test_add_rs);
    MU_RUN_TEST(test_get_version_rs);
    MU_RUN_TEST(test_greeting_rs_null_pointer);
    MU_RUN_TEST(test_greeting_rs);
    MU_RUN_TEST(test_add_struct);
}

int
main(void) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return minunit_fail;
}
