#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "test.h"
#include "c_vector.h"

bool test_make_cap_vect(void);
bool test_destroy_vect(void);
bool test_push(void);
bool test_get(void);
bool test_trim(void);
bool test_resize(void);
bool test_pop(void);
bool test_peek(void);

static ftest tests[] = {
    test_make_cap_vect,
    test_destroy_vect,
    test_push,
    test_get,
    test_trim,
    test_resize,
    test_pop,
    test_peek
};
//------------------------------------------------------------------------------

bool test_make_cap_vect(void)
{
    c_vector vect_, * vect = &vect_;

    check(!c_vect_make_cap(vect, 5, 0));
    check(!c_vect_make_cap(vect, -1, 5));
    check(!c_vect_make_cap(vect, 5, -1));
    check(!c_vect_make_cap(vect, -23, 5));
    check(!c_vect_make_cap(vect, 0, 5));

    int elem_size = sizeof(double);
    const int capacity = 5;
    check(c_vect_make_cap(vect, elem_size, capacity) == vect);
    check(vect->arr);
    check(vect->elem_size == elem_size);
    check(vect->capacity == capacity);
    check(vect->elem_count == 0);

    free(vect->arr);
    return true;
}
//------------------------------------------------------------------------------

bool test_destroy_vect(void)
{
    int elem_size = sizeof(double);
    const int capacity = 5;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, elem_size, capacity) == vect);
    check(vect->arr);
    check(c_vect_elem_size(vect) == elem_size);
    check(c_vect_capacity(vect) == capacity);
    check(c_vect_length(vect) == 0);

    c_vect_destroy(vect);
    check(vect->capacity == 0);
    check(vect->elem_count == 0);
    check(vect->elem_size == 0);
    check(vect->arr == 0);

    check(c_vect_make_cap(vect, elem_size, capacity) == vect);
    check(vect->arr);
    check(c_vect_elem_size(vect) == elem_size);
    check(c_vect_capacity(vect) == capacity);
    check(c_vect_length(vect) == 0);
    c_vect_destroy_null(vect);
    check(!vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_push(void)
{
    int esz = sizeof(double);
    int cap = 1;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);

    double n = 5;
    typedef unsigned char byte;
    void * dest = (byte *)vect->arr + vect->elem_count * vect->elem_size;
    check(c_vect_push(vect, &n) == dest);
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == cap);

    dest = (byte *)vect->arr + vect->elem_count * vect->elem_size;
    check(c_vect_push(vect, &n) == dest);
    check(c_vect_length(vect) == 2);

    int gr = C_VECT_GROWTH_RATE;
    check(c_vect_capacity(vect) == cap*gr);

    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 3);
    check(c_vect_capacity(vect) == cap*gr*gr);

    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 4);
    check(c_vect_capacity(vect) == cap*gr*gr);

    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 5);
    check(c_vect_capacity(vect) == cap*gr*gr*gr);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_get(void)
{
    int esz = sizeof(double);
    int cap = 1;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);

    double n = 5;
    void * result = c_vect_push(vect, &n);
    check(result);

    n = 6;
    result = c_vect_push(vect, &n);
    check(result);

    n = 7;
    result = c_vect_push(vect, &n);
    check(result);

    check(c_vect_length(vect) == 3);
    double * ip = c_vect_get(vect, -1);
    check(!ip);
    ip = c_vect_get(vect, 100);
    check(!ip);
    ip = c_vect_get(vect, 2);
    check(ip);
    check(*ip == 7);
    ip = c_vect_get(vect, 1);
    check(ip);
    check(*ip == 6);
    ip = c_vect_get(vect, 0);
    check(ip);
    check(*ip == 5);

    check(c_vect_data(vect) == ip);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_trim(void)
{
    int esz = sizeof(double);
    int cap = 1;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);

    double n = 5;
    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == cap);

    n = 6;
    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 2);
    check(c_vect_capacity(vect) == cap*C_VECT_GROWTH_RATE);
    double * get = c_vect_get(vect, 1);
    check(*get == n);

    check(c_vect_trim(vect) == vect);
    check(c_vect_capacity(vect) == cap*C_VECT_GROWTH_RATE);
    get = c_vect_get(vect, 1);
    check(*get == n);

    n = 7;
    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 3);
    check(c_vect_capacity(vect) == cap*C_VECT_GROWTH_RATE*C_VECT_GROWTH_RATE);
    get = c_vect_get(vect, 2);
    check(*get == n);

    check(c_vect_trim(vect) == vect);
    check(c_vect_capacity(vect) == 3);
    check(c_vect_capacity(vect) == c_vect_length(vect));
    get = c_vect_get(vect, 2);
    check(*get == n);

    n = 8;
    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 4);
    check(c_vect_capacity(vect) == 3*C_VECT_GROWTH_RATE);
    get = c_vect_get(vect, 3);
    check(*get == n);

    n = 9;
    check(c_vect_push(vect, &n));
    check(c_vect_length(vect) == 5);
    check(c_vect_capacity(vect) == 3*C_VECT_GROWTH_RATE);
    get = c_vect_get(vect, 4);
    check(*get == n);

    check(c_vect_trim(vect) == vect);
    check(c_vect_capacity(vect) == 5);
    check(c_vect_capacity(vect) == c_vect_length(vect));
    get = c_vect_get(vect, 2);
    check(*get == 7);
    get = c_vect_get(vect, 4);
    check(*get == n);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_resize(void)
{
    int esz = sizeof(double);
    int cap = 1;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);

    check(!c_vect_resize(vect, 0));

    double n = 5;
    void * result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == cap);

    check(!c_vect_resize(vect, -1));
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == cap);

    int new_sz = 5;
    check(c_vect_resize(vect, new_sz));
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == new_sz);
    double * val = c_vect_get(vect, 0);
    check(val);
    check(*val == n);

    n = 6;
    result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 2);
    check(c_vect_capacity(vect) == new_sz);

    check(!c_vect_resize(vect, 0));
    check(c_vect_length(vect) == 2);
    check(c_vect_capacity(vect) == new_sz);

    check(!c_vect_resize(vect, -200));
    check(c_vect_length(vect) == 2);
    check(c_vect_capacity(vect) == new_sz);

    n = 7;
    result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 3);
    check(c_vect_capacity(vect) == new_sz);

    new_sz = 3;
    check(c_vect_resize(vect, new_sz));
    check(c_vect_length(vect) == 3);
    check(c_vect_capacity(vect) == new_sz);
    val = c_vect_get(vect, 2);
    check(val);
    check(*val == n);

    new_sz = 2;
    check(c_vect_resize(vect, new_sz));
    check(c_vect_length(vect) == new_sz);
    check(c_vect_capacity(vect) == new_sz);
    val = c_vect_get(vect, 1);
    check(val);
    check(*val == n-1);

    c_vect_destroy(vect);

    check(c_vect_make(vect, esz) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == C_VECT_DEFAULT_CAPACITY);
    check(c_vect_length(vect) == 0);

    new_sz = 5;
    check(c_vect_resize(vect, new_sz));
    check(c_vect_length(vect) == 0);
    check(c_vect_capacity(vect) == new_sz);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_pop(void)
{
    int esz = sizeof(double);
    int cap = 5;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);

    check(!c_vect_pop(vect));
    check(c_vect_length(vect) == 0);

    double n = 5;
    void * result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == cap);

    n = 6;
    result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 2);
    check(c_vect_capacity(vect) == cap);

    check(c_vect_pop(vect) == vect);
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == cap);

    check(c_vect_pop(vect) == vect);
    check(c_vect_length(vect) == 0);
    check(c_vect_capacity(vect) == cap);

    check(!c_vect_pop(vect));
    check(c_vect_length(vect) == 0);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_peek(void)
{
    int esz = sizeof(double);
    int cap = 5;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);

    void * back = c_vect_peek(vect);
    check(!back);

    double n = 5;
    void * result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 1);
    check(c_vect_capacity(vect) == cap);
    back = c_vect_peek(vect);
    check(back == result);
    check(*((double *)back) == n);


    n = 6;
    result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 2);
    back = c_vect_peek(vect);
    check(back == result);
    check(*((double *)back) == n);

    n = 7;
    result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 3);
    back = c_vect_peek(vect);
    check(back == result);
    check(*((double *)back) == n);

    n = 8;
    result = c_vect_push(vect, &n);
    check(result);
    check(*((double *)result) == n);
    check(c_vect_length(vect) == 4);
    back = c_vect_peek(vect);
    check(back == result);
    check(*((double *)back) == n);

    c_vect_pop(vect);
    back = c_vect_peek(vect);
    check(c_vect_length(vect) == 3);
    check(*((double *)back) == n-1);


    c_vect_pop(vect);
    back = c_vect_peek(vect);
    check(c_vect_length(vect) == 2);
    check(*((double *)back) == n-2);
    check(c_vect_capacity(vect) == 5);

    c_vect_resize(vect, 1);
    back = c_vect_peek(vect);
    check(c_vect_length(vect) == 1);
    check(*((double *)back) == n-3);
    check(c_vect_capacity(vect) == 1);

    while (c_vect_pop(vect))
        continue;

    check(!c_vect_peek(vect));

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

void run_tests(void)
{
    int i, end = sizeof(tests)/sizeof(*tests);

    int passed = 0;
    for (i = 0; i < end; ++i)
        if (tests[i]())
            ++passed;

    if (passed != end)
        putchar('\n');

    int failed = end - passed;
    report(passed, failed);
    return;
}
//------------------------------------------------------------------------------

int main(void)
{
	run_tests();
	return 0;
}
//------------------------------------------------------------------------------
