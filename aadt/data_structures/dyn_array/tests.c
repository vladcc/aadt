#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "test.h"
#include "dynamic_array.h"

void run_tests(void);

bool dyn_arr_destroy_test(void);
bool dyn_arr_create_test(void);
bool dyn_arr_getters_test(void);
bool dyn_trim_test(void);
bool dyn_arr_push_back_test(void);
bool dyn_arr_pop_back_test(void);
bool dyn_arr_write_at_index_test(void);
bool dyn_arr_insert_at_index_test(void);
bool dyn_arr_remove_at_index_test(void);
bool dyn_arr_read_at_index_test(void);
bool dyn_arr_read_back_test(void);
bool dyn_arr_sort_test(void);
bool dyn_arr_find_from_test(void);
bool dyn_arr_remove_by_value_test(void);
bool dyn_arr_append_array_test(void);
bool dyn_arr_apply_test(void);
bool dyn_arr_insert_online_test(void);
bool dyn_arr_reset_test(void);
bool dyn_arr_const_apply_test(void);
bool dyn_arr_const_find_from_test(void);
bool dyn_arr_is_empty_test(void);
bool dyn_arr_const_data_test(void);
bool dyn_arr_stress_test(void);

void * emalloc(int nbytes);

ftest tests[] = {
    dyn_arr_destroy_test,
    dyn_arr_create_test,
    dyn_arr_getters_test,
    dyn_trim_test,
    dyn_arr_push_back_test,
    dyn_arr_pop_back_test,
    dyn_arr_write_at_index_test,
    dyn_arr_insert_at_index_test,
    dyn_arr_remove_at_index_test,
    dyn_arr_read_at_index_test,
    dyn_arr_read_back_test,
    dyn_arr_sort_test,
    dyn_arr_find_from_test,
    dyn_arr_remove_by_value_test,
    dyn_arr_append_array_test,
    dyn_arr_apply_test,
    dyn_arr_insert_online_test,
    dyn_arr_reset_test,
    dyn_arr_const_apply_test,
    dyn_arr_const_find_from_test,
    dyn_arr_is_empty_test,
    dyn_arr_const_data_test,
    //dyn_arr_stress_test,
};
//---------------------------------------------------------------------------

int main(int argc, char * argv[])
{
    run_tests();
    return 0;
}
//---------------------------------------------------------------------------

int compar(const void * key1, const void * key2)
{
    const int * k1 = key1;
    const int * k2 = key2;

    if (*k1 < *k2)
        return -1;
    if (*k1 > *k2)
        return 1;

    return 0;
}
//---------------------------------------------------------------------------

bool dyn_arr_destroy_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(DYN_ARR_NULL_PTR == dyn_arr_destroy(NULL));

    vector->capacity = 1;
    vector->elm_size = 1;
    vector->compar = (void *)1;
    vector->destroy = NULL;
    vector->elements = 1;
    vector->data = emalloc(1);
    vector->sorted = 1;

    check(0 == dyn_arr_destroy(vector));

    check(0 == vector->capacity);
    check(0 == vector->elm_size);
    check(0 == vector->compar);
    check(0 == vector->elements);
    check(0 == vector->data);
    check(0 == vector->sorted);

    check(DYN_ARR_BAD_ARRAY == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

void add_1(void * elem)
{
    int * p = *(int **)elem;
    *p += 1;
    return;
}

bool dyn_arr_create_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(DYN_ARR_NULL_PTR == dyn_arr_create(NULL, 1, sizeof(int), NULL, NULL));
    check(DYN_ARR_BAD_INPUT == dyn_arr_create(vector, 0, sizeof(int), NULL, NULL));
    check(DYN_ARR_BAD_INPUT == dyn_arr_create(vector, 1, 0, NULL, NULL));

    check(0 == dyn_arr_create(vector, 5, sizeof(int), NULL, NULL));
    check(5 == vector->capacity);
    check(sizeof(int) == vector->elm_size);
    check(0 == vector->elements);
    check(1 == vector->sorted); // sorted

    check(0 == dyn_arr_destroy(vector));

    int a_ = 1, b_ = 2, c_ = 3;
    int * a = &a_, * b = &b_, * c = &c_;
    check(0 == dyn_arr_create(vector, 5, sizeof(int *), NULL, add_1));
    check(1 == vector->sorted); // sorted
    check(0 == dyn_arr_push_back(vector, &a));
    check(0 == vector->sorted); // not sorted
    check(0 == dyn_arr_push_back(vector, &b));
    check(0 == dyn_arr_push_back(vector, &c));
    check(3 == dyn_arr_elements(vector));
    check(0 == dyn_arr_destroy(vector));
    check(2 == a_);
    check(3 == b_);
    check(4 == c_);
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_getters_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;
    void * data_out;
    check(DYN_ARR_NULL_PTR == dyn_arr_capacity(NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_elements(NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_element_size(NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_data(NULL, &data_out));
    check(DYN_ARR_NULL_PTR == dyn_arr_data(vector, NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_is_sorted(NULL));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_capacity(vector));
    check(DYN_ARR_BAD_ARRAY == dyn_arr_elements(vector));
    check(DYN_ARR_BAD_ARRAY == dyn_arr_element_size(vector));
    check(DYN_ARR_BAD_ARRAY == dyn_arr_data(vector, &data_out));
    check(DYN_ARR_BAD_ARRAY == dyn_arr_is_sorted(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    const int stop = 5;
    int i;
    for (i = 0; i < stop; ++i)
        dyn_arr_push_back(vector, &i);

    for (i = 0; i < stop; ++i)
        check(((int *)vector->data)[i] == i);

    check(vector->elements == stop);

    check(DYN_ARR_DEFAULT_CAPACITY == dyn_arr_capacity(vector));
    check(stop == dyn_arr_elements(vector));
    check(sizeof(int) == dyn_arr_element_size(vector));
    check(0 == dyn_arr_data(vector, &data_out));
    check(vector->data == data_out);
    check(0 == dyn_arr_is_sorted(vector));

    check(0 == dyn_arr_destroy(vector));

    return true;
}
//---------------------------------------------------------------------------

bool dyn_trim_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    const int capcity = 8;

    check(DYN_ARR_NULL_PTR == dyn_arr_trim(NULL));
    check(0 == dyn_arr_create(vector, capcity, sizeof(int), NULL, NULL));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL

    check(DYN_ARR_BAD_ARRAY == dyn_arr_trim(vector));
    vector->data = emalloc(sizeof(int));
    check(DYN_ARR_BAD_ARRAY == dyn_arr_trim(vector));
    vector->elements = 32;
    check(DYN_ARR_BAD_ARRAY == dyn_arr_trim(vector));
    vector->elm_size = 0x7FFFFFFF;
    check(DYN_ARR_MEM_ERR == dyn_arr_trim(vector));
    vector->capacity = 16;
    vector->elements = vector->capacity;
    check(0 == dyn_arr_trim(vector));

    check(0 == dyn_arr_create(vector, capcity, sizeof(int), NULL, NULL));

    check(capcity == vector->capacity);
    check(DYN_ARR_INDEX_ERR == dyn_arr_trim(vector));
    vector->elements = 5;
    check(5 == vector->elements);
    check(0 == dyn_arr_trim(vector));
    check(vector->elements == vector->capacity);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_push_back_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;
    int n = 6;

    check(DYN_ARR_NULL_PTR == dyn_arr_push_back(NULL, &n));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_push_back(vector, NULL));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_push_back(vector, &n));

    check(0 == dyn_arr_create(vector, 2, sizeof(int), NULL, NULL));
    check(1 == vector->sorted);
    check(0 == vector->elements);
    check(2 == vector->capacity);
    check(0 == dyn_arr_push_back(vector, &n));
    check(0 == vector->sorted);
    check(1 == vector->elements);
    check(2 == vector->capacity);
    ++n;
    check(0 == dyn_arr_push_back(vector, &n));
    check(2 == vector->elements);
    check(2 == vector->capacity);
    ++n;
    check(0 == dyn_arr_push_back(vector, &n));
    check(3 == vector->elements);
    check(4 == vector->capacity);

    check(((int *)vector->data)[0] == 6);
    check(((int *)vector->data)[1] == 7);
    check(((int *)vector->data)[2] == 8);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_pop_back_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;
    int n = 6;
    void * last;

    check(DYN_ARR_NULL_PTR == dyn_arr_pop_back(NULL, &last));
    check(DYN_ARR_NULL_PTR == dyn_arr_pop_back(vector, NULL));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_pop_back(vector, &last));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_pop_back(vector, &last)); // vector->elements = 0
    check(0 == dyn_arr_push_back(vector, &n));
    check(1 == vector->elements);
    check(((int *)vector->data)[0] == 6);
    check(0 == dyn_arr_pop_back(vector, &last));
    check(*((int *)last) == 6);
    check(0 == vector->elements);

    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_write_at_index_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;
    int n = 6;

    check(DYN_ARR_NULL_PTR == dyn_arr_write_at_index(NULL, 0, &n));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_write_at_index(vector, 0, NULL));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_write_at_index(vector, 0, &n));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_write_at_index(vector, 0, &n));
    check(0 == dyn_arr_push_back(vector, &n));
    check(((int *)vector->data)[0] == 6);
    ++n;
    check(0 == dyn_arr_write_at_index(vector, 0, &n));
    check(((int *)vector->data)[0] == 7);
    check(DYN_ARR_INDEX_ERR == dyn_arr_write_at_index(vector, 1, &n));
    check(DYN_ARR_INDEX_ERR == dyn_arr_write_at_index(vector, 5, &n));
    check(DYN_ARR_INDEX_ERR == dyn_arr_write_at_index(vector, -1, &n));

    check(0 == dyn_arr_destroy(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(true == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_push_back(vector, &n));
    check(false == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_sort(vector));
    check(true == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_write_at_index(vector, 0, &n));
    check(false == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_insert_at_index_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;
    int n = 6;

    check(DYN_ARR_NULL_PTR == dyn_arr_insert_at_index(NULL, 0, &n));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_insert_at_index(vector, 0, NULL));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_insert_at_index(vector, 0, &n));

    check(0 == dyn_arr_create(vector, 2, sizeof(int), NULL, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_insert_at_index(vector, -1, &n));
    check(DYN_ARR_INDEX_ERR == dyn_arr_insert_at_index(vector, 0, &n));
    check(DYN_ARR_INDEX_ERR == dyn_arr_insert_at_index(vector, 1, &n));
    check(0 == dyn_arr_push_back(vector, &n));
    check(((int *)vector->data)[0] == 6);
    ++n; // 7
    check(0 == dyn_arr_insert_at_index(vector, 0, &n));
    check(((int *)vector->data)[0] == 7);
    check(((int *)vector->data)[1] == 6);
    ++n; // 8
    check(0 == dyn_arr_insert_at_index(vector, 0, &n));
    check(((int *)vector->data)[0] == 8);
    check(((int *)vector->data)[1] == 7);
    check(((int *)vector->data)[2] == 6);
    check(3 == vector->elements);
    check(4 == vector->capacity);
    ++n; // 9
    check(DYN_ARR_INDEX_ERR == dyn_arr_insert_at_index(vector, 3, &n));
    check(3 == vector->elements);
    check(4 == vector->capacity);
    check(0 == dyn_arr_insert_at_index(vector, 1, &n));
    check(4 == vector->elements);
    check(4 == vector->capacity);
    check(((int *)vector->data)[0] == 8);
    check(((int *)vector->data)[1] == 9);
    check(((int *)vector->data)[2] == 7);
    check(((int *)vector->data)[3] == 6);
    ++n; // 10
    check(0 == dyn_arr_insert_at_index(vector, 3, &n));
    check(5 == vector->elements);
    check(8 == vector->capacity);
    check(((int *)vector->data)[0] == 8);
    check(((int *)vector->data)[1] == 9);
    check(((int *)vector->data)[2] == 7);
    check(((int *)vector->data)[3] == 10);
    check(((int *)vector->data)[4] == 6);
    void * last;
    check (0 == dyn_arr_pop_back(vector, &last));
    check(*((int *)last) == 6);
    check(4 == vector->elements);
    check(8 == vector->capacity);
    check(((int *)vector->data)[0] == 8);
    check(((int *)vector->data)[1] == 9);
    check(((int *)vector->data)[2] == 7);
    check(((int *)vector->data)[3] == 10);
    check(0 == dyn_arr_trim(vector));
    check(4 == vector->elements);
    check(4 == vector->capacity);
    check(((int *)vector->data)[0] == 8);
    check(((int *)vector->data)[1] == 9);
    check(((int *)vector->data)[2] == 7);
    check(((int *)vector->data)[3] == 10);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_remove_at_index_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(DYN_ARR_NULL_PTR == dyn_arr_remove_at_index(NULL, 0));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_remove_at_index(vector, -1));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_remove_at_index(vector, 0));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_remove_at_index(vector, 0));
    check(DYN_ARR_INDEX_ERR == dyn_arr_remove_at_index(vector, 10));

    const int stop = 5;
    int i;
    for (i = 0; i < stop; ++i)
        dyn_arr_push_back(vector, &i);

    for (i = 0; i < stop; ++i)
        check(((int *)vector->data)[i] == i);

    check(vector->elements == stop);
    check(DYN_ARR_INDEX_ERR == dyn_arr_remove_at_index(vector, 10));

    // remove 0
    check(0 == dyn_arr_remove_at_index(vector, 0));
    for (i = 0; i < stop-1; ++i)
        check(((int *)vector->data)[i] == i+1);
    check(vector->elements == stop-1);

    // remove 4
    check(0 == dyn_arr_remove_at_index(vector, vector->elements-1));
    for (i = 0; i < stop-2; ++i)
        check(((int *)vector->data)[i] == i+1);
    check(vector->elements == stop-2);

    // remove 2
    check(0 == dyn_arr_remove_at_index(vector, 1));
    check(((int *)vector->data)[0] == 1);
    check(((int *)vector->data)[1] == 3);
    check(2 == vector->elements);

    i = 2;
    check(0 == dyn_arr_insert_at_index(vector, 1, &i));
    check(((int *)vector->data)[0] == 1);
    check(((int *)vector->data)[1] == 2);
    check(((int *)vector->data)[2] == 3);
    check(3 == vector->elements);
    i = 4;
    check(0 == dyn_arr_push_back(vector, &i));
    for (i = 0; i < stop-1; ++i)
        check(((int *)vector->data)[i] == i+1);
    check(vector->elements == stop-1);
    check(4 == vector->elements);
    check(DYN_ARR_DEFAULT_CAPACITY == vector->capacity);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_read_at_index_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;
    const void * _out;
    const void ** out = &_out;

    check(DYN_ARR_NULL_PTR == dyn_arr_read_at_index(NULL, 0, out));
    check(DYN_ARR_NULL_PTR == dyn_arr_read_at_index(vector, 0, NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_read_at_index(vector, -1, out));
    check(DYN_ARR_INDEX_ERR == dyn_arr_read_at_index(vector, 0, out));
    check(DYN_ARR_INDEX_ERR == dyn_arr_read_at_index(vector, 10, out));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_read_at_index(vector, 0, out));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    const int stop = 5;
    int i;
    for (i = 0; i < stop; ++i)
        dyn_arr_push_back(vector, &i);

    for (i = 0; i < stop; ++i)
        check(((int *)vector->data)[i] == i);

    check(vector->elements == stop);

    check(DYN_ARR_INDEX_ERR == dyn_arr_read_at_index(vector, -1, out));
    check(DYN_ARR_INDEX_ERR == dyn_arr_read_at_index(vector, 10, out));
    check(0 == dyn_arr_read_at_index(vector, 0, out));
    check(*((int *)_out) == 0);
    check(0 == dyn_arr_read_at_index(vector, 2, out));
    check(*((int *)_out) == 2);
    check(0 == dyn_arr_read_at_index(vector, 4, out));
    check(*((int *)_out) == 4);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_read_back_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;
    void * out;

    check(DYN_ARR_NULL_PTR == dyn_arr_read_back(NULL, (const void **)&out));
    check(DYN_ARR_NULL_PTR == dyn_arr_read_back(vector, NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_read_back(vector, (const void **)&out));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_read_back(vector, (const void **)&out));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    const int stop = 5;
    int i;
    for (i = 0; i < stop; ++i)
        dyn_arr_push_back(vector, &i);

    for (i = 0; i < stop; ++i)
        check(((int *)vector->data)[i] == i);

    check(0 == dyn_arr_read_back(vector, (const void **)&out));
    check(*((int *)out) == stop-1);
    check(vector->elements == stop);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_sort_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(DYN_ARR_NULL_PTR == dyn_arr_sort(NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_sort(vector));
    check(0 == dyn_arr_destroy(vector)); // vector->data = NULL
    vector->compar = (void *)1; // compar gets checked before data
    check(DYN_ARR_BAD_ARRAY == dyn_arr_sort(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(0 == dyn_arr_sort(vector));
    int n = 6;
    check(0 == dyn_arr_push_back(vector, &n));
    check(false == dyn_arr_is_sorted(vector));
    check(1 == dyn_arr_elements(vector));
    check(0 == dyn_arr_sort(vector));
    check(true == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_destroy(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    const int stop = 5;
    int i;
    for (i = 0; i < stop; ++i)
        dyn_arr_push_back(vector, &i);
    check(0 == dyn_arr_is_sorted(vector)); // not sorted

    for (i = 0; i < stop; ++i)
        check(((int *)vector->data)[i] == i);
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector));  // sorted
    check(0 == dyn_arr_destroy(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    for (i = stop; i >= 0; --i)
        dyn_arr_push_back(vector, &i);
    check(0 == vector->sorted);
    check(vector->elements == stop+1);

    for (i = 0; i < stop; ++i)
        check(((int *)vector->data)[i] == stop - i);
    check(0 == dyn_arr_sort(vector));
    check(1 == vector->sorted);

    for (i = 0; i <= stop; ++i)
        check(((int *)vector->data)[i] == i);
    check(0 == dyn_arr_destroy(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
     for (i = 0; i < stop; ++i)
        dyn_arr_push_back(vector, &i);
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector));  // sorted

    i = 0;
    check(0 == dyn_arr_insert_at_index(vector, 0, &i)); // 0, 0, 1, 2, 3, 4
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector));  // sorted

    i = 1;
    check(0 == dyn_arr_write_at_index(vector, 1, &i)); // 0, 1, 1, 2, 3, 4
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector));  // sorted

    i = -5;
    check(0 == dyn_arr_insert_at_index(vector, 4, &i)); // 0, 1, 1, 2, -5, 3, 4
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector));  // sorted

    i = 123;
    check(0 == dyn_arr_write_at_index(vector, 1, &i)); // -5, 123, 1, 1, 2, 3, 4
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector));  // sorted

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_find_from_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    int n = 6;
    int stop = 5;
    int * found;
    check(DYN_ARR_NULL_PTR == dyn_arr_find_from(NULL, 0, &n, (void **)&found));
    check(DYN_ARR_NULL_PTR == dyn_arr_find_from(vector, 0, NULL, (void **)&found));
    check(DYN_ARR_NULL_PTR== dyn_arr_find_from(vector, 0, &n, NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_find_from(vector, -123, &n, (void **)&found));
    check(0 == dyn_arr_destroy(vector)); // darr->data = NULL
    vector->compar = (void *)1; // compar checks before data
    check(DYN_ARR_BAD_ARRAY == dyn_arr_find_from(vector, -123, &n, (void **)&found));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(1 == dyn_arr_is_sorted(vector)); //sorted
    int i;
    for (i = stop; i >= 0; --i)
        dyn_arr_push_back(vector, &i);
    dyn_arr_push_back(vector, &i);
    i = 0;
    dyn_arr_push_back(vector, &i);  // 5, 4, 3, 2, 1, 0, -1, 0
    check(0 == dyn_arr_is_sorted(vector)); // not sorted



    n = 5;
    check(DYN_ARR_INDEX_ERR == dyn_arr_find_from(vector, -123, &n, (void **)&found));
    check(DYN_ARR_INDEX_ERR == dyn_arr_find_from(vector, 200, &n, (void **)&found));

    // linear search
    check(0 == dyn_arr_find_from(vector, 0, &n, (void **)&found));
    check(n == *found);
    check(DYN_ARR_NOT_FOUND == dyn_arr_find_from(vector, 1, &n, (void **)&found));
    check(NULL == found);
    n = 2;
    check(3 == dyn_arr_find_from(vector, 0, &n, (void **)&found));
    check(n == *found);
    n = 0;
    check(5 == dyn_arr_find_from(vector, 0, &n, (void **)&found));
    check(n == *found);
    found = NULL;
    check(7 == dyn_arr_find_from(vector, 6, &n, (void **)&found));
    check(n == *found);
    n = -1;
    check(6 == dyn_arr_find_from(vector, 6, &n, (void **)&found));
    check(n == *found);
    check(DYN_ARR_NOT_FOUND == dyn_arr_find_from(vector, 7, &n, (void **)&found));
    check(NULL == found);

    n = -10023;
    check(DYN_ARR_NOT_FOUND == dyn_arr_find(vector, &n, (void **)&found));
    check(NULL == found);
    n = 10023;
    check(DYN_ARR_NOT_FOUND == dyn_arr_find(vector, &n, (void **)&found));
    check(NULL == found);

    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector));  // sorted: -1, 0, 0, 1, 2, 3, 4, 5

    // binary search
    n = -1;
    check(0 == dyn_arr_find_from(vector, 0, &n, (void **)&found));
    check(false == dyn_arr_is_sorted(vector));
    check(n == *found);

    check(0 == dyn_arr_sort(vector));
    check(true == dyn_arr_is_sorted(vector));

    check(DYN_ARR_NOT_FOUND == dyn_arr_const_find_from(vector, 1, &n, (const void **)&found));
    check(true == dyn_arr_is_sorted(vector));
    check(NULL == found);
    n = 0;
    check(1 == dyn_arr_const_find_from(vector, 0, &n, (const void **)&found));
    check(n == *found);
    check(2 == dyn_arr_const_find_from(vector, 2, &n, (const void **)&found));
    check(n == *found);
    found = NULL;
    check(DYN_ARR_NOT_FOUND == dyn_arr_const_find_from(vector, 6, &n, (const void **)&found));
    check(NULL == found);
    n = 5;
    check(7 == dyn_arr_const_find_from(vector, 0, &n, (const void **)&found));
    check(n == *found);
    check(7 == dyn_arr_const_find_from(vector, 2, &n, (const void **)&found));
    check(n == *found);

    n = -10023;
    check(DYN_ARR_NOT_FOUND == dyn_arr_const_find(vector, &n, (const void **)&found));
    check(NULL == found);
    n = 10023;
    check(DYN_ARR_NOT_FOUND == dyn_arr_const_find(vector, &n, (const void **)&found));
    check(NULL == found);

    check(0 == dyn_arr_destroy(vector));
    return true;
}

bool dyn_arr_const_find_from_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    int n = 6;
    int * found;
    check(DYN_ARR_NULL_PTR == dyn_arr_const_find_from(NULL, 0, &n, (const void **)&found));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(1 == dyn_arr_is_sorted(vector)); //sorted
    check(0 == dyn_arr_push_back(vector, &n));
    ++n;
    check(0 == dyn_arr_push_back(vector, &n));
    check(0 == dyn_arr_is_sorted(vector)); //not sorted
    check(1 == dyn_arr_const_find_from(vector, 0, &n, (const void **)&found));
    check(0 == dyn_arr_is_sorted(vector)); //not sorted
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector)); //sorted
    check(1 == dyn_arr_const_find_from(vector, 0, &n, (const void **)&found));
    check(1 == dyn_arr_is_sorted(vector)); //sorted
    n = 123421;
    check(DYN_ARR_NOT_FOUND == dyn_arr_const_find(vector, &n, (const void **)&found));
    check(1 == dyn_arr_is_sorted(vector)); //sorted

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_remove_by_value_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    int n = 6;
    check(DYN_ARR_NULL_PTR == dyn_arr_remove_by_value(NULL, &n));
    check(DYN_ARR_NULL_PTR == dyn_arr_remove_by_value(vector, NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(0 == dyn_arr_destroy(vector)); // vector->compar == NULL
    check(DYN_ARR_NULL_PTR == dyn_arr_remove_by_value(vector, &n));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(true == dyn_arr_is_sorted(vector));

    int i;
    int stop = 5;
    for (i = stop; i >= 0; --i)
        dyn_arr_push_back(vector, &i); // 5, 4, 3, 2, 1, 0
    check(6 == dyn_arr_elements(vector));

    n = 100;
    check(DYN_ARR_NOT_FOUND == dyn_arr_remove_by_value(vector, &n));
    n = -123;
    check(DYN_ARR_NOT_FOUND == dyn_arr_remove_by_value(vector, &n));
    n = 5;
    check(0 == dyn_arr_remove_by_value(vector, &n));
    check(5 == dyn_arr_elements(vector));
    for (i = 0, n = 4; i < 5; ++i, --n)
        check(((int *)vector->data)[i] == n); //4, 3, 2, 1, 0

    n = 0;
    check(0 == dyn_arr_remove_by_value(vector, &n));
    check(4 == dyn_arr_elements(vector));
    for (i = 0, n = 4; i < 4; ++i, --n)
        check(((int *)vector->data)[i] == n); //4, 3, 2, 1

    n = 2;
    check(false == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_sort(vector));
    check(true == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_remove_by_value(vector, &n)); //1, 3, 4
    check(true == dyn_arr_is_sorted(vector));
    check(3 == dyn_arr_elements(vector));
    check(((int *)vector->data)[0] == 1);
    check(((int *)vector->data)[1] == 3);
    check(((int *)vector->data)[2] == 4);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_append_array_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    int iarr[] = {1, 2, 3, 4, 5};
    int size_iarr = sizeof(iarr)/sizeof(*iarr);
    check(DYN_ARR_NULL_PTR == dyn_arr_append_array(NULL, iarr, sizeof(*iarr), size_iarr));
    check(DYN_ARR_NULL_PTR == dyn_arr_append_array(vector, NULL, sizeof(*iarr), size_iarr));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(DYN_ARR_BAD_INPUT == dyn_arr_append_array(vector, iarr, -123, size_iarr));
    check(DYN_ARR_BAD_INPUT == dyn_arr_append_array(vector, iarr, sizeof(*iarr), -42542));
    check(0 == dyn_arr_destroy(vector)); // darr->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_append_array(vector, iarr, sizeof(*iarr), size_iarr));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(1 == dyn_arr_is_sorted(vector)); // sorted
    check(0 == dyn_arr_append_array(vector, iarr, sizeof(*iarr), size_iarr));
    check(size_iarr == dyn_arr_elements(vector));
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(DYN_ARR_DEFAULT_CAPACITY == dyn_arr_capacity(vector));

    int i;
    for(i = 0; i < size_iarr; ++i)
        check(((int *)vector->data)[i] == iarr[i]);
    check(0 == dyn_arr_destroy(vector));

    check(0 == dyn_arr_create(vector, 4, sizeof(int), compar, NULL));
    check(1 == dyn_arr_is_sorted(vector)); // sorted
    int n;
    for (n = -2; n <= 0; ++n)
        check(0 == dyn_arr_push_back(vector, &n));
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(3 == dyn_arr_elements(vector));
    check(4 == dyn_arr_capacity(vector));
    check(0 == dyn_arr_append_array(vector, iarr, sizeof(*iarr), size_iarr));
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(3 + size_iarr == dyn_arr_elements(vector));
    check(4 * 2 == dyn_arr_capacity(vector));

    check(((int *)vector->data)[0] == -2);
    check(((int *)vector->data)[1] == -1);
    check(((int *)vector->data)[2] == 0);
    for(i = 0; i < size_iarr; ++i)
        check(((int *)vector->data)[i+3] == iarr[i]);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

void foo(void * arr_elm, void * args)
{
    int * elm = arr_elm;
    *elm += (int)args;
    return;
}

bool dyn_arr_apply_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    int iarr[] = {1, 2, 3, 4, 5};
    int size_iarr = sizeof(iarr)/sizeof(*iarr);
    check(DYN_ARR_NULL_PTR == dyn_arr_apply_to(NULL, 0, foo, (void *)5));
    check(DYN_ARR_NULL_PTR == dyn_arr_apply_to(vector, 0, NULL, (void *)5));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(DYN_ARR_INDEX_ERR == dyn_arr_apply_to(vector, -5, foo, (void *)5));
    check(0 == dyn_arr_destroy(vector)); // darr->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_apply_to(vector, 0, foo, (void *)5));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(0 == dyn_arr_append_array(vector, iarr, sizeof(*iarr), size_iarr));
    check(size_iarr == dyn_arr_elements(vector));
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    int i;
    for(i = 0; i < size_iarr; ++i)
        check(((int *)vector->data)[i] == iarr[i]);

    check(DYN_ARR_INDEX_ERR == dyn_arr_apply_to(vector, -200, foo, (void *)1));
    check(DYN_ARR_INDEX_ERR == dyn_arr_apply_to(vector, 3000, foo, (void *)1));
    check(0 == dyn_arr_apply_to(vector, 0, foo, (void *)1));

    check(((int *)vector->data)[0] == iarr[0] + 1);
    check(--((int *)vector->data)[0] == iarr[0]);

    check(0 == dyn_arr_apply(vector, foo, (void *)6));
    for(i = 0; i < size_iarr; ++i)
        check(((int *)vector->data)[i] == iarr[i] + 6);

    check(0 == dyn_arr_destroy(vector));
    return true;
}

void foo_const(const void * arr_elm, void * args)
{
    return;
}

bool dyn_arr_const_apply_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(DYN_ARR_NULL_PTR == dyn_arr_const_apply_to(NULL, 0, foo_const, NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(1 == dyn_arr_is_sorted(vector)); // sorted
    int n = 4;
    check(0 == dyn_arr_push_back(vector, &n));
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_const_apply_to(vector, 0, foo_const, (void *)1));
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_sort(vector));
    check(1 == dyn_arr_is_sorted(vector)); // sorted
    check(0 == dyn_arr_const_apply(vector, foo_const, (void *)1));
    check(1 == dyn_arr_is_sorted(vector)); // sorted

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool is_sorted(int * arr, int size)
{
    int i;
    for(i = 1; i < size; ++i)
        if (arr[i-1] > arr[i])
            return false;

    return true;
}

bool dyn_arr_insert_online_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    int n = 1;
    check(DYN_ARR_NULL_PTR == dyn_arr_insert_online(NULL, &n));
    check(DYN_ARR_NULL_PTR == dyn_arr_insert_online(vector, NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(DYN_ARR_NULL_PTR == dyn_arr_insert_online(vector, &n));
    check(0 == dyn_arr_destroy(vector));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(0 == dyn_arr_destroy(vector)); // darr->data = NULL
    vector->compar = (void *)1; // compar checks before data
    check(DYN_ARR_BAD_ARRAY == dyn_arr_insert_online(vector, &n));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(1 == dyn_arr_is_sorted(vector)); // sorted
    check(0 == dyn_arr_insert_online(vector, &n));
    ++n;
    check(0 == dyn_arr_push_back(vector, &n));
    check(2 == dyn_arr_elements(vector));
    check(0 == dyn_arr_is_sorted(vector)); // not sorted
    check(0 == dyn_arr_sort(vector));

    int arr[] = {5, 4, 5, 3, 3, 5, 2, 1, 0, 1, 0, 0, 2, 1, 5, 5, -100};
    int arr_size = sizeof(arr)/sizeof(*arr);

    for (int i = 0; i < arr_size; ++i)
    {
        check(0 == dyn_arr_insert_online(vector, &arr[i]));
        void * data;
        dyn_arr_const_data(vector, (const void **)&data);
        if (!is_sorted(data, dyn_arr_elements(vector)))
            check(is_sorted(data, dyn_arr_elements(vector)));
    }
    check(2 + arr_size == dyn_arr_elements(vector));

    n = 10;
    check(0 == dyn_arr_insert_online(vector, &n));
    check(0 == dyn_arr_insert_online(vector, &n));
    check(0 == dyn_arr_insert_online(vector, &n));
    check(((int *)vector->data)[arr_size + 2] == n);
    check(((int *)vector->data)[arr_size + 1 + 2] == n);
    check(((int *)vector->data)[arr_size + 2 + 2] == n);

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_reset_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(DYN_ARR_NULL_PTR == dyn_arr_reset(NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(0 == dyn_arr_destroy(vector)); // darr->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_reset(vector));

    int iarr[] = {1, 2, 3, 4, 5};
    int size_iarr = sizeof(iarr)/sizeof(*iarr);
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(0 == dyn_arr_append_array(vector, iarr, sizeof(*iarr), size_iarr));
    check(size_iarr == dyn_arr_elements(vector));
    check(false == dyn_arr_is_sorted(vector));

    check(0 == dyn_arr_reset(vector));
    check(0 == dyn_arr_elements(vector));
    check(true == dyn_arr_is_sorted(vector));

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_is_empty_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(DYN_ARR_NULL_PTR == dyn_arr_is_empty(NULL));
    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(0 == dyn_arr_destroy(vector)); // darr->data = NULL
    check(DYN_ARR_BAD_ARRAY == dyn_arr_is_empty(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), NULL, NULL));
    check(true == dyn_arr_is_empty(vector));
    int n = 999;
    check(0 == dyn_arr_push_back(vector, &n));
    check(false == dyn_arr_is_empty(vector));
    check(0 == dyn_arr_destroy(vector));
    check(DYN_ARR_BAD_ARRAY == dyn_arr_is_empty(vector));

    return true;
}
//---------------------------------------------------------------------------

bool dyn_arr_const_data_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    const void * cdata = NULL;
    void * data = NULL;

    check(DYN_ARR_NULL_PTR == dyn_arr_const_data(NULL, &cdata));
    check(DYN_ARR_NULL_PTR == dyn_arr_const_data(vector, NULL));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    check(true == dyn_arr_is_sorted(vector));

    check(0 == dyn_arr_data(vector, &data));
    check(data == vector->data);
    check(false == dyn_arr_is_sorted(vector));

    check(0 == dyn_arr_const_data(vector, &cdata));
    check(cdata == vector->data);
    check(false == dyn_arr_is_sorted(vector));

    cdata = NULL;
    check(0 == dyn_arr_sort(vector));
    check(true == dyn_arr_is_sorted(vector));
    check(0 == dyn_arr_const_data(vector, &cdata));
    check(cdata == vector->data);
    check(true == dyn_arr_is_sorted(vector));

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

#define MAX (1000 * 1000 * 100)
bool dyn_arr_stress_test(void)
{
    Dyn_arr _vector;
    Dyn_arr * vector = &_vector;

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));

    int i;
    for (i = 0; i < MAX; ++i)
        dyn_arr_push_back(vector, &i);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    dyn_arr_trim(vector);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    dyn_arr_reset(vector);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    for (i = 0; i < MAX; ++i)
        dyn_arr_insert_online(vector, &i);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    dyn_arr_sort(vector);

    check(0 == dyn_arr_destroy(vector));

    check(0 == dyn_arr_create(vector, DYN_ARR_DEFAULT_CAPACITY, sizeof(int), compar, NULL));
    for (i = 0; i < MAX; ++i)
        dyn_arr_push_back(vector, &i);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    int * out;
    while (0 == dyn_arr_pop_back(vector, (void **)&out))
        continue;
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    dyn_arr_trim(vector);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    dyn_arr_push_back(vector, out);
    dyn_arr_trim(vector);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    for (i = 0; i < MAX; ++i)
        dyn_arr_push_back(vector, (void **)&i);
    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    puts("read at vs pointer");
    int * pi, * tmp;
    dyn_arr_data(vector, (void **)&pi);
    int a, b;
    a = 0;
    
    clock_t begin, end;
    double elapsed;
    
    begin = clock();
    for (i = 0; i < MAX; ++i)
    {
        dyn_arr_read_at_index(vector, i, (const void **)&tmp);
        a += *tmp;
    }
	end = clock();
	elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("accumulated: %d, time %.3f sec\n", a, elapsed);

    b = 0;
    begin = clock();
    for (i = 0; i < MAX; ++i)
    {
        b += pi[i];
    }   
	end = clock();
	elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("accumulated: %d, time %.3f sec\n", b, elapsed);

    dyn_arr_remove_at_index(vector, 0);
    printf("elements: %d\n", dyn_arr_elements(vector));
    dyn_arr_remove_at_index(vector, dyn_arr_elements(vector) / 2);
    printf("elements: %d\n", dyn_arr_elements(vector));

    for (i = 0; i < MAX; ++i)
        dyn_arr_remove_at_index(vector, dyn_arr_elements(vector) - 1);

    printf("elements: %d\n", dyn_arr_elements(vector));
    printf("capacity: %d\n", dyn_arr_capacity(vector));

    check(0 == dyn_arr_destroy(vector));
    return true;
}
//---------------------------------------------------------------------------

void run_tests(void)
{
    int i, end = sizeof(tests)/sizeof(*tests);

    int passed = 0;
    for (i = 0; i < end; ++i)
        if (tests[i]())
            ++passed;

    if (passed != end)
        putchar('\n');
	
	report(passed, end-passed);
    return;
}
//---------------------------------------------------------------------------

void * emalloc(int nbytes)
{
	void * memp = malloc(nbytes);

	if (NULL == memp)
    {
        fprintf(stderr, "Memory allocation failed in %s()\n", __func__);
        exit(EXIT_FAILURE);
    }

	return memp;
}
//---------------------------------------------------------------------------
