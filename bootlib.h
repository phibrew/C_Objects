#ifndef BOOTLIB_H
#define BOOTLIB_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
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
#define assert_ptr_not_equal(a, b, msg) munit_assert_ptr_not_equal(a, b)
#define assert_ptr(a, op, b, msg) munit_assert_ptr(a, op, b)
#define assert_ptr_null(ptr, msg) munit_assert_null(ptr)
#define assert_false(expr) munit_assert_false(expr)
#define assert_not_null(ptr, msg) munit_assert_not_null(ptr)

#define assert_float(a, op, b, msg) munit_assert_double(a, op, b)
#define assert_float_equal(a, b, msg) munit_assert_double_equal(a, ==, b)
#define assert_string_equal(a, b, msg) munit_assert_string_equal(a, b)
// Translates the Boot.dev suite builder
#define munit_suite(name, tests) { (char*)name, tests, NULL, 1, MUNIT_SUITE_OPTION_NONE }


extern size_t _boot_allocated_bytes;

static inline void *boot_malloc(size_t size){
    void *ptr = malloc(size + sizeof(size_t));
    if(!ptr) return NULL;

    *(size_t*)ptr = size;
    _boot_allocated_bytes += size;

    return (char*)ptr + sizeof(size_t);
}

static inline void boot_free(void *ptr){
    if(!ptr) return;
    void *real_ptr = (char*)ptr - sizeof(size_t);

    _boot_allocated_bytes -= *(size_t*)real_ptr;
    free(real_ptr);
}

static inline void *boot_calloc(size_t count, size_t size){
    size_t tot_size = size*count;

    void *ptr = boot_malloc(tot_size);
    if(!ptr) return NULL;

    char *c = (char*)ptr;
    for(size_t i = 0; i<tot_size; ++i, c+=1){
        *c = 0;
    }
    return ptr;
}

static inline bool boot_all_freed() {
    return _boot_allocated_bytes == 0;
}

static inline int boot_alloc_size() {
    return (int)_boot_allocated_bytes;
}

#define malloc boot_malloc
#define free boot_free
#define calloc boot_calloc
#endif