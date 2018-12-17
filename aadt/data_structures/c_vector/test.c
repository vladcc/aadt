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
bool test_find(void);
bool test_is_sorted(void);
bool test_sort(void);
bool test_bsearch(void);
bool test_insert_online(void);
bool test_reset_empty(void);
bool test_append_array(void);
bool test_write_at_index(void);
bool test_set_compar(void);
bool test_insert_at_index(void);
bool test_remove_at_index(void);
bool test_apply_args(void);
bool test_apply(void);
bool test_zero_out(void);
bool test_set_length(void);
bool test_copy(void);

static ftest tests[] = {
    test_make_cap_vect,
    test_destroy_vect,
    test_push,
    test_get,
    test_trim,
    test_resize,
    test_pop,
    test_peek,
    test_find,
    test_is_sorted,
    test_sort,
    test_bsearch,
    test_insert_online,
    test_reset_empty,
    test_append_array,
    test_write_at_index,
    test_set_compar,
    test_insert_at_index,
    test_remove_at_index,
    test_apply_args,
    test_apply,
    test_zero_out,
    test_set_length,
    test_copy
};

typedef unsigned char byte;
//------------------------------------------------------------------------------

int compar(const void * k1, const void * k2)
{
    int * i1 = (int *)k1;
    int * i2 = (int *)k2;

    int result = 0;
    if (*i1 > *i2)
        result = 1;

    if (*i1 < *i2)
        result = -1;

    return result;
}
//------------------------------------------------------------------------------

bool test_copy(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    check(c_vect_make(vect, esz, compar) == vect);

    int end = c_vect_capacity(vect) - 5;
    for (int i = 0; i < end; ++i)
        c_vect_push(vect, &i);

    c_vector cpy_, * cpy = &cpy_;
    cpy->elem_count = -1;
    cpy->elem_size = -1;
    cpy->arr = NULL;
    cpy->capacity = -1;
    cpy->compar = NULL;

    check(c_vect_length(cpy) != c_vect_length(vect));
    check(c_vect_elem_size(cpy) != c_vect_elem_size(vect));
    check(c_vect_data(cpy) != c_vect_data(vect));
    check(c_vect_capacity(cpy) != c_vect_capacity(vect));
    check(cpy->compar != vect->compar);

    check(c_vect_copy(cpy, vect) == cpy);
    check(c_vect_length(cpy) == c_vect_length(vect));
    check(c_vect_elem_size(cpy) == c_vect_elem_size(vect));
    check(c_vect_data(cpy) != c_vect_data(vect));
    check(c_vect_capacity(cpy) == c_vect_capacity(vect));
    check(cpy->compar == vect->compar);

    end = c_vect_length(cpy);
    for (int i = 0; i < end; ++i)
        check(*((int *)c_vect_get(cpy, i)) == *((int *)c_vect_get(vect, i)));

    end = c_vect_capacity(cpy);
    c_vect_set_length(cpy, end);
    check(c_vect_length(cpy) == c_vect_capacity(cpy));
    c_vect_set_length(vect, end);
    check(c_vect_length(vect) == c_vect_capacity(vect));
    check(c_vect_length(vect) == c_vect_length(cpy));
    for (int i = 0; i < end; ++i)
        check(*((int *)c_vect_get(cpy, i)) == *((int *)c_vect_get(vect, i)));
    c_vect_destroy(cpy);

    check(c_vect_length(cpy) != c_vect_length(vect));
    check(c_vect_elem_size(cpy) != c_vect_elem_size(vect));
    check(c_vect_data(cpy) != c_vect_data(vect));
    check(c_vect_capacity(cpy) != c_vect_capacity(vect));
    check(cpy->compar != vect->compar);

    check(c_vect_copy(cpy, vect) == cpy);
    check(c_vect_length(cpy) == c_vect_length(vect));
    check(c_vect_elem_size(cpy) == c_vect_elem_size(vect));
    check(c_vect_data(cpy) != c_vect_data(vect));
    check(c_vect_capacity(cpy) == c_vect_capacity(vect));
    check(cpy->compar == vect->compar);

    end = c_vect_length(cpy);
    for (int i = 0; i < end; ++i)
        check(*((int *)c_vect_get(cpy, i)) == *((int *)c_vect_get(vect, i)));

    c_vect_destroy(vect);
    c_vect_destroy(cpy);
    return true;
}

//------------------------------------------------------------------------------

bool test_set_length(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    int cap = 4;
    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);

    check(c_vect_length(vect) == 0);
    int len = 2;
    check(c_vect_set_length(vect, -200) == NULL);
    check(c_vect_set_length(vect, 100) == NULL);
    check(c_vect_set_length(vect, len) == vect);
    check(c_vect_length(vect) == len);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_set_length(vect, -200) == NULL);
    check(c_vect_set_length(vect, 100) == NULL);
    check(c_vect_set_length(vect, cap) == vect);
    check(c_vect_length(vect) == cap);
    check(c_vect_push(vect, &len) == c_vect_peek(vect));
    check(c_vect_length(vect) == cap+1);
    check(c_vect_pop(vect));
    check(c_vect_length(vect) == cap);
    check(c_vect_set_length(vect, -200) == NULL);
    check(c_vect_set_length(vect, 100) == NULL);
    check(c_vect_capacity(vect) == cap*2);

    check(c_vect_zero_out(vect) == vect);
    int all = c_vect_length(vect);
    for (int i = 0; i < all; ++i)
        check(*((int *)c_vect_get(vect, i)) == 0);

    byte * data = c_vect_data(vect);
    all = c_vect_capacity(vect) * c_vect_elem_size(vect);
    for (int i = 0; i < all; ++i)
        check(data[i] == 0);

    c_vect_destroy(vect);

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
    check(c_vect_zero_out(vect) == vect);

    data = c_vect_data(vect);
    all = c_vect_capacity(vect) * c_vect_elem_size(vect);
    for (int i = 0; i < all; ++i)
        check(data[i] == 0);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_zero_out(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;

    check(c_vect_make(vect, esz, NULL) == vect);

    int i, all = 5;
    for (i = 0; i < all; ++i)
    {
        c_vect_push(vect, &i);
        check(*((int *)c_vect_get(vect, i)) == i);
    }

    check(c_vect_length(vect) == all);
    check(c_vect_zero_out(vect) == vect);
    for (i = 0; i < all; ++i)
        check(*((int *)c_vect_get(vect, i)) == 0);

    c_vect_push(vect, &all);
    check(c_vect_length(vect) == all+1);
    check(*((int *)c_vect_get(vect, all)) == all);

    c_vect_destroy(vect);

    check(c_vect_make(vect, esz, NULL) == vect);
    int cap = c_vect_capacity(vect);

    for (i = 0; i < cap; ++i)
    {
        c_vect_push(vect, &i);
        check(*((int *)c_vect_get(vect, i)) == i);
    }
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == cap);
    check(c_vect_pop(vect) == vect);
    check(c_vect_pop(vect) == vect);
    check(c_vect_pop(vect) == vect);
    check(c_vect_length(vect) <= cap);

    check(c_vect_zero_out(vect) == vect);
    byte * data = c_vect_data(vect);
    all = c_vect_capacity(vect) * c_vect_elem_size(vect);
    for (int i = 0; i < all; ++i)
        check(data[i] == 0);


    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

void add5(void * arr_elm)
{
    int * elm = arr_elm;
    *elm += 5;
}

bool test_apply(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    int cap = 4;
    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);

    int max = 10;
    for (int i = 0; i < max; ++i)
        c_vect_push(vect, &i);

    for (int i = 0; i < max; ++i)
        check(*((int *)c_vect_get(vect, i)) == i);

    int m = 5;
    check(c_vect_apply(vect, add5) == vect);
    for (int i = 0; i < max; ++i)
        check(*((int *)c_vect_get(vect, i)) == i+m);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

typedef struct add_args {
    int n;
} add_args;

void addn(void * arr_elm, void * args)
{
    int * elm = arr_elm;
    add_args * padd = args;
    *elm += padd->n;
}

bool test_apply_args(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    int cap = 4;
    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);

    int max = 10;
    for (int i = 0; i < max; ++i)
        c_vect_push(vect, &i);

    for (int i = 0; i < max; ++i)
        check(*((int *)c_vect_get(vect, i)) == i);

    int n = 1;
    add_args aa = {n};
    check(c_vect_apply_args(vect, addn, &aa) == vect);
    for (int i = 0; i < max; ++i)
        check(*((int *)c_vect_get(vect, i)) == i+n);

    int m = 5;
    aa.n = m;
    check(c_vect_apply_args(vect, addn, &aa) == vect);
    for (int i = 0; i < max; ++i)
        check(*((int *)c_vect_get(vect, i)) == i+n+m);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_remove_at_index(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    int cap = 4;
    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);

    int max = 10;
    for (int i = 0; i < max; ++i)
        c_vect_push(vect, &i);

    check(c_vect_remove_at_index(vect, -123) == NULL);
    check(c_vect_remove_at_index(vect, 123) == NULL);

    int rind = 4;
    int * n = c_vect_get(vect, rind);
    check(*n == 4);
    check(c_vect_remove_at_index(vect, rind) == vect);
    check(*n == 5);


    rind = 0;
    n = c_vect_get(vect, rind);
    check(*n == 0);
    check(c_vect_remove_at_index(vect, rind) == vect);
    check(*n == 1);

    rind = c_vect_length(vect) - 1;
    n = c_vect_get(vect, rind);
    check(*n == 9);
    check(c_vect_remove_at_index(vect, rind) == vect);
    check(c_vect_length(vect) == 7);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_insert_at_index(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    int cap = 4;
    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);

    check(c_vect_capacity(vect) == cap);

    int i, offs = 5;
    for (i = 1; i < cap; ++i)
    {
        int j = i * 2 + offs;
        c_vect_push(vect, &j);
    }

    check(c_vect_capacity(vect) == cap);
    int * first = c_vect_get(vect, 0);
    check(*first == 2+offs);

    int n = 0;
    check(c_vect_insert_at_index(vect, -123, &n) == NULL);
    check(c_vect_insert_at_index(vect, 123, &n) == NULL);

    check(c_vect_insert_at_index(vect, 0, &n) == c_vect_data(vect));
    first = c_vect_get(vect, 0);
    check(*first == n);
    check(c_vect_capacity(vect) == cap);

    n = 10;
    int last = c_vect_length(vect) - 1, * pe;
    check(c_vect_insert_at_index(vect, last, &n) == (int *)vect->arr + last);
    check(((int *)vect->arr)[last] == n);
    check(*(pe = c_vect_get(vect, last)) == n);
    check(c_vect_capacity(vect) == cap*2);

    int mid = 2;
    n = 8;
    check(c_vect_insert_at_index(vect, mid, &n) == (int *)vect->arr + mid);
    check(((int *)vect->arr)[mid] == n);
    check(*(pe = c_vect_get(vect, mid)) == n);

    check(c_vect_set_compar(vect, compar) == vect);
    check(c_vect_is_sorted(vect));
    check(c_vect_capacity(vect) == cap*2);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_write_at_index(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    check(c_vect_make(vect, esz, NULL) == vect);

    int n = 6;
    check(c_vect_write_at_index(vect, -1, &n) == NULL);
    check(c_vect_write_at_index(vect, -123, &n) == NULL);
    check(c_vect_write_at_index(vect, 124, &n) == NULL);

    int cap = c_vect_capacity(vect);
    check(c_vect_write_at_index(vect, cap, &n) == NULL);

    check(vect->arr == c_vect_data(vect));
    check(c_vect_is_empty(vect));
    check(c_vect_write_at_index(vect, 0, &n) == NULL);

    int i;
    for (i = 0; i < 8; ++i)
        c_vect_push(vect, &i);

    check(c_vect_write_at_index(vect, -1, &n) == NULL);
    check(c_vect_write_at_index(vect, -123, &n) == NULL);
    check(c_vect_write_at_index(vect, 124, &n) == NULL);

    check(c_vect_write_at_index(vect, 0, &n) == c_vect_data(vect));
    check(((int *)vect->arr)[0] == n);
    int * pe;
    check(*(pe = c_vect_get(vect, 0)) == n);

    int len = c_vect_length(vect);
    check(c_vect_write_at_index(vect, len, &n) == NULL);
    --len;
    check(c_vect_write_at_index(vect, len, &n) == ((int *)vect->arr) + len);
    check(((int *)vect->arr)[len] == n);
    check(*(pe = c_vect_get(vect, len)) == n);

    int mid = 5;
    check(c_vect_write_at_index(vect, mid, &n) == ((int *)vect->arr) + mid);
    check(((int *)vect->arr)[mid] == n);
    check(*(pe = c_vect_get(vect, mid)) == n);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_append_array(void)
{
    int esz = sizeof(int);
    int cap = 1;
    c_vector vect_, * vect = &vect_;
    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);

    int iarr[] = {4, 5, 6, 7, 8};
    int size_iarr = sizeof(iarr)/sizeof(*iarr);

    check(c_vect_capacity(vect) == cap);
    check(c_vect_append_array(vect, iarr, size_iarr));
    check(c_vect_capacity(vect) == 8);

    int i, * elem;
    for(i = 0; i < size_iarr; ++i)
        check(*(elem = c_vect_get(vect, i)) == iarr[i]);

    c_vect_destroy(vect);

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
    int iend = 4;
    for (i = 0; i < iend; ++i)
        c_vect_push(vect, &i);

    check(c_vect_length(vect) == i);
    check(c_vect_capacity(vect) == i);
    check(c_vect_append_array(vect, iarr, size_iarr));
    check(c_vect_capacity(vect) == 16);

    for (i = 0; i < iend; ++i)
        check(*(elem = c_vect_get(vect, i)) == i);

    for(int n = 0; n < size_iarr; ++n, ++i)
        check(*(elem = c_vect_get(vect, i)) == iarr[n]);

    check(c_vect_length(vect) == iend+size_iarr);

    c_vect_push(vect, &i);
    for (i = 0; i < iend; ++i)
        check(*(elem = c_vect_get(vect, i)) == i);

    for(int n = 0; n < size_iarr; ++n, ++i)
        check(*(elem = c_vect_get(vect, i)) == iarr[n]);

    check(*(elem = c_vect_get(vect, c_vect_length(vect)-1)) == i);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_set_compar(void)
{
    int esz = sizeof(int);
    c_vector vect_, * vect = &vect_;
    check(c_vect_make(vect, esz, NULL) == vect);

    check(vect->compar == NULL);
    check(c_vect_set_compar(vect, compar) == vect);
    check(vect->compar == compar);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_reset_empty(void)
{
    int esz = sizeof(int);
    int cap = 1;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, compar, cap) == vect);
    check(c_vect_is_empty(vect));

    for (int i = 0; i < 10; ++i)
        c_vect_push(vect, &i);
    check(!c_vect_is_empty(vect));

    check(c_vect_reset(vect) == vect);
    check(c_vect_is_empty(vect));

    for (int i = 0; i < 10; ++i)
        c_vect_push(vect, &i);
    check(!c_vect_is_empty(vect));

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_insert_online(void)
{
    int esz = sizeof(int);
    int cap = 1;
    c_vector vect_, * vect = &vect_;
    check(c_vect_make_cap(vect, esz, compar, cap) == vect);

    int n = 1, * ins = NULL;
    check(*(ins = c_vect_insert_online(vect, &n)) == n);
    check(c_vect_is_sorted(vect) == vect);
    ++n;
    check(*(ins = c_vect_insert_online(vect, &n)) == n);
    check(c_vect_is_sorted(vect) == vect);

    int arr[] = {5, 4, 5, 3, 3, 5, 2, 1, 0, 1, 0, 0, 2, 1, 5, 5, -100};
    int arr_size = sizeof(arr)/sizeof(*arr);

    int i = 0, ind;
    for (; i < arr_size; ++i)
    {
        check(*(ins = c_vect_insert_online_ind(vect, arr + i, &ind)) == arr[i]);
        check(*(ins = c_vect_get(vect, ind)) == arr[i]);
        check(c_vect_is_sorted(vect) == vect);
    }
    check(2 + arr_size == c_vect_length(vect));

    n = 10;
    check(*(ins = c_vect_insert_online(vect, &n)) == n);
    check(c_vect_is_sorted(vect) == vect);

    check(*(ins = c_vect_insert_online_ind(vect, &n, &ind)) == n);
    check(*(ins = c_vect_get(vect, ind)) == n);
    check(c_vect_is_sorted(vect) == vect);

    check(*(ins = c_vect_insert_online(vect, &n)) == n);
    check(c_vect_is_sorted(vect) == vect);

    int * elem;
    check(*(elem = c_vect_get(vect, arr_size + 2)) == n);
    check(*(elem = c_vect_get(vect, arr_size + 1 + 2)) == n);
    check(*(elem = c_vect_get(vect, arr_size + 2 + 2)) == n);

    check(((int *)vect->arr)[arr_size + 2] == n);
    check(((int *)vect->arr)[arr_size + 1 + 2] == n);
    check(((int *)vect->arr)[arr_size + 2 + 2] == n);

    c_vect_destroy(vect);

    check(c_vect_make_cap(vect, esz, compar, cap) == vect);
    int newarr[] = {7, 8, 0, 1, 3, 6, 0, 4, 5, 3};

    arr_size = sizeof(newarr)/sizeof(*newarr);
    for (int i = 0; i < arr_size; ++i)
    {
        int ind;
        if (!c_vect_bsearch_ind(vect, newarr + i, &ind))
            check(*(ins = c_vect_insert_online(vect, newarr + i)) == newarr[i]);
    }

    check(c_vect_is_sorted(vect));
    int checkarr[] = {0, 1, 3, 4, 5, 6, 7, 8};

    arr_size = sizeof(checkarr)/sizeof(*checkarr);
    for (int i = 0; i < arr_size; ++i)
        check(*(elem = c_vect_get(vect, i)) == checkarr[i]);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_bsearch(void)
{
    int esz = sizeof(int);
    int cap = 1;
    c_vector vect_, * vect = &vect_;
    check(c_vect_make_cap(vect, esz, compar, cap) == vect);

    int * key;
    check(!c_vect_bsearch_ind(vect, NULL, NULL));

    for (int i = 0; i < 10; ++i)
        c_vect_push(vect, &i);

    int n = 9;
    c_vect_push(vect, &n);
    n = 123;
    c_vect_push(vect, &n);
    check(c_vect_is_sorted(vect) == vect);

    struct search {
        int n;
        int pos;
        bool res;
    } tbl[] = {
        {-864, -1, false}, {89, -1, false}, {0, 0, true},
        {9, 10, true}, {123, 11, true}, {5, 5, true}
    };

    for (unsigned int i = 0; i < sizeof(tbl)/sizeof(*tbl); ++i)
    {
        key = &(tbl[i].n);
        void * res = c_vect_bsearch_ind(vect, key, NULL);
        check((bool)res == tbl[i].res);
        if (res)
            check(compar(res, key) == 0);

        int pos;
        res = c_vect_bsearch_ind(vect, key, &pos);
        check((bool)res == tbl[i].res);

        bool right_pos = (pos == tbl[i].pos);
        if (!right_pos)
            printf("n: %d, pos :%d\n", tbl[i].n, tbl[i].pos);

        check(right_pos);
    }

    void * first = c_vect_get(vect, 0);
    void * last = c_vect_peek(vect);
    int ind;
    void * ffirst = c_vect_bsearch_ind(vect, first, &ind);
    check(ffirst == first);
    check(0 == ind);
    check(compar(first, ffirst) == 0);

    void * flast = c_vect_bsearch_ind(vect, last, &ind);
    check(flast == last);
    check(c_vect_length(vect)-1 == ind);
    check(compar(flast, last) == 0);

    int less = -93471, more = 946242;
    int * out = c_vect_bsearch_ind(vect, &less, &ind);
    check(NULL == out);
    check(-1 == ind);

    out = c_vect_bsearch_ind(vect, &more, &ind);
    check(NULL == out);
    check(-1 == ind);

    c_vect_destroy(vect);

    check(c_vect_make_cap(vect, esz, compar, cap) == vect);
    n = 1;
    int i;
    for (i = 0; i < 10; ++i)
        c_vect_push(vect, &n);

    out = c_vect_bsearch_ind(vect, &n, &ind);
    check(out == c_vect_bsearch(vect, &n));
    check(*out == n);
    check(ind == (i-1)/2);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_is_sorted(void)
{
    int esz = sizeof(int);
    int cap = 1;
    c_vector vect_, * vect = &vect_;
    check(c_vect_make_cap(vect, esz, compar, cap) == vect);

    check(c_vect_is_sorted(vect) == NULL);
    int n = 10;
    c_vect_push(vect, &n);
    check(c_vect_length(vect) == 1);
    check(c_vect_is_sorted(vect) == vect);

    n = 9;
    c_vect_push(vect, &n);
    check(c_vect_is_sorted(vect) == NULL);
    c_vect_destroy(vect);

    check(c_vect_make_cap(vect, esz, compar, cap) == vect);

    for (int i = 0; i < 10; ++i)
        c_vect_push(vect, &i);
    check(c_vect_is_sorted(vect) == vect);

    n = 9;
    c_vect_push(vect, &n);
    check(c_vect_is_sorted(vect) == vect);

    n = 5;
    c_vect_push(vect, &n);
    check(c_vect_is_sorted(vect) == NULL);

    n = 19;
    c_vect_push(vect, &n);
    check(c_vect_is_sorted(vect) == NULL);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_sort(void)
{
    int esz = sizeof(int);
    int cap = 1;
    c_vector vect_, * vect = &vect_;
    check(c_vect_make_cap(vect, esz, compar, cap) == vect);

    for (int i = 10; i >= 0; --i)
        c_vect_push(vect, &i);
    check(c_vect_is_sorted(vect) == NULL);

    check(c_vect_sort(vect) == vect);
    check(c_vect_is_sorted(vect) == vect);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_find(void)
{
    int esz = sizeof(int);
    int cap = 1;
    c_vector vect_, * vect = &vect_;

    check(c_vect_make_cap(vect, esz, compar, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(vect->compar == compar);

    int pos;
    for (int i = 10, j = 0; i >= 0; --i, ++j)
    {
        void * dest = (byte *)vect->arr + vect->elem_count * vect->elem_size;
        check(c_vect_push(vect, &i) == dest);
        check(c_vect_find_ind(vect, &i, NULL) == dest);
        check(c_vect_find(vect, &i) == dest);
        check(c_vect_find_ind(vect, &i, &pos) == dest);
        check(pos == j);
    }

    int n = 10;
    check(c_vect_find_ind(vect, &n, NULL) == c_vect_data(vect));
    check(c_vect_find_ind(vect, &n, &pos));
    check(pos == 0);

    n = -123;
    check(c_vect_find_ind(vect, &n, NULL) == NULL);
    check(c_vect_find_ind(vect, &n, &pos) == NULL);
    check(pos == -1);

    n = 35290;
    check(c_vect_find_ind(vect, &n, NULL) == NULL);
    check(c_vect_find_ind(vect, &n, &pos) == NULL);
    check(pos == -1);

    c_vect_destroy(vect);
    return true;
}
//------------------------------------------------------------------------------

bool test_make_cap_vect(void)
{
    c_vector vect_, * vect = &vect_;

    check(!c_vect_make_cap(vect, 5, NULL, 0));
    check(!c_vect_make_cap(vect, -1, NULL, 5));
    check(!c_vect_make_cap(vect, 5, NULL, -1));
    check(!c_vect_make_cap(vect, -23, NULL, 5));
    check(!c_vect_make_cap(vect, 0, NULL, 5));

    int elem_size = sizeof(double);
    const int capacity = 5;
    check(c_vect_make_cap(vect, elem_size, compar, capacity) == vect);
    check(vect->arr);
    check(vect->compar == compar);
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

    check(c_vect_make_cap(vect, elem_size, NULL, capacity) == vect);
    check(vect->arr);
    check(c_vect_elem_size(vect) == elem_size);
    check(c_vect_capacity(vect) == capacity);
    check(c_vect_length(vect) == 0);

    c_vect_destroy(vect);
    check(vect->capacity == 0);
    check(vect->elem_count == 0);
    check(vect->elem_size == 0);
    check(vect->arr == 0);

    check(c_vect_make_cap(vect, elem_size, NULL, capacity) == vect);
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

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);

    double n = 5;
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

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);
    check(c_vect_get(vect, -1) == NULL);
    check(c_vect_get(vect, 0) == NULL);
    check(c_vect_get(vect, 1) == NULL);
    check(c_vect_get(vect, 5) == NULL);
    check(c_vect_get(vect, 3000) == NULL);

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

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
    check(c_vect_elem_size(vect)== esz);
    check(c_vect_capacity(vect) == cap);
    check(c_vect_length(vect) == 0);
    check(c_vect_trim(vect) == NULL);

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

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
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

    check(c_vect_make(vect, esz, NULL) == vect);
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

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
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

    check(c_vect_make_cap(vect, esz, NULL, cap) == vect);
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
