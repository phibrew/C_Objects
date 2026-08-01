#ifndef BOOTLIB_H
#define BOOTLIB_H

#include <stdlib.h>
#include <stdbool.h>
#include "munit.h"

// 1. Mock the RUN/SUBMIT flags
#define RUN 1
#define SUBMIT 2

// 2. Munit macros
#define munit_case(type, name, ...) \
static MunitResult name(const MunitParameter params[], void* data) { \
    (void)params; \
    (void)data; \
    __VA_ARGS__ \
    return MUNIT_OK; \
}

#define munit_test(path, func) { (char*)path, func, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
#define munit_null_test { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }

// Translates Boot.dev asserts into standard munit asserts (ignoring the custom message)
#define assert_int(a, op, b, msg) munit_assert_int(a, op, b)
#define assert_ptr_not_null(ptr, msg) munit_assert_not_null(ptr)
#define assert_ptr_equal(ptr, ad, msg) munit_assert_ptr_equal(ptr, ad)
#define assert_size(a, op, b, msg) munit_assert_size(a, op, b)
#define assert_int_equal(a, b, msg) munit_assert_int(a, ==, b)
#define assert_null(ptr, msg) munit_assert_null(ptr)
// Translates the Boot.dev suite builder
#define munit_suite(name, tests) { (char*)name, tests, NULL, 1, MUNIT_SUITE_OPTION_NONE }


#endif