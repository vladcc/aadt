#include <stdlib.h>                 // malloc(), free(), qsort()
#include <string.h>                 // memcpy(), memset(), memmove()
#include <stdbool.h>
#include "dynamic_array.h"
//---------------------------------------------------------------------------

#ifdef DYN_ARR_COMPILE_CHECKS
    #define sanity_chk(expr, retval)  if (expr) return (retval)
#else
    #define sanity_chk(expr, retval)
#endif

typedef unsigned char byte;
//---------------------------------------------------------------------------

static int dyn_arr_grow(Dyn_arr * darr)
{
    sanity_chk(NULL == darr->data ||
               darr->capacity < 1 ||
               darr->elm_size < 1,
               DYN_ARR_BAD_ARRAY);

    void * reall = realloc(darr->data, darr->capacity * darr->elm_size * DYN_ARR_GROWTH_RATE);

    if (NULL == reall)
        return DYN_ARR_MEM_ERR;

    darr->data = reall;
    darr->capacity = darr->capacity * DYN_ARR_GROWTH_RATE;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_create(Dyn_arr * darr, int capacity, int element_size,
                   int (*compar)(const void * key1, const void * key2),
                   void (*destroy)(void * elem))
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(capacity < 1 || element_size < 1, DYN_ARR_BAD_INPUT);

    if (NULL == (darr->data = malloc(capacity * element_size)))
        return DYN_ARR_MEM_ERR;

    darr->compar = compar;
    darr->destroy = destroy;
    darr->capacity = capacity;
    darr->elm_size = element_size;
    darr->elements = 0;
    darr->sorted = true;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_destroy(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    if (darr->destroy != NULL)
        dyn_arr_apply(darr, (void (*)(void *, void *))darr->destroy, NULL);

    free(darr->data);
    memset(darr, 0, sizeof(*darr));
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_capacity(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    return darr->capacity;
}

int dyn_arr_elements(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    return darr->elements;
}

int dyn_arr_element_size(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    return darr->elm_size;
}

int dyn_arr_const_data(Dyn_arr * darr, const void ** data_out)
{
    sanity_chk(NULL == darr || NULL == data_out, DYN_ARR_NULL_PTR);

    int sorted = darr->sorted;
    int ret = dyn_arr_data(darr, (void **)data_out);
    darr->sorted = sorted;
    return ret;
}

int dyn_arr_data(Dyn_arr * darr, void ** data_out)
{
    sanity_chk(NULL == darr || NULL == data_out, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    darr->sorted = false;
    *data_out = darr->data;
    return 0;
}

int dyn_arr_is_sorted(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    return darr->sorted;
}

int dyn_arr_is_empty(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    return 0 == darr->elements;
}
//---------------------------------------------------------------------------

int dyn_arr_append_array(Dyn_arr * darr, void * arr, int elem_size, int len)
{
    sanity_chk(NULL == darr || NULL == arr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(elem_size < 1 || len < 1, DYN_ARR_BAD_INPUT);

    while (darr->capacity < darr->elements + len)
        if (dyn_arr_grow(darr) != 0)
            return DYN_ARR_MEM_ERR;

    darr->sorted = false;
    memcpy((byte *)darr->data + darr->elements * darr->elm_size,
           (byte *)arr,
           elem_size * len);

    darr->elements += len;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_trim(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data || darr->elm_size < 1, DYN_ARR_BAD_ARRAY);
    sanity_chk(0 == darr->elements, DYN_ARR_INDEX_ERR);

    if (darr->elements == darr->capacity)
        return 0;

    void * reall = realloc(darr->data,
                           darr->elements * darr->elm_size);

    if (NULL == reall)
        return DYN_ARR_MEM_ERR;

    darr->data = reall;
    darr->capacity = darr->elements;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_reset(Dyn_arr * darr)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    darr->elements = 0;
    darr->sorted = true;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_push_back(Dyn_arr * darr, const void * key)
{
    sanity_chk(NULL == darr || NULL == key, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    if (darr->elements == darr->capacity)
        if (dyn_arr_grow(darr) != 0)
            return DYN_ARR_MEM_ERR;

    darr->sorted = false;
    memcpy((byte *)darr->data + darr->elements * darr->elm_size,
           (byte *)key,
           darr->elm_size);
    darr->elements++;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_pop_back(Dyn_arr * darr, void ** last_out)
{
    sanity_chk(NULL == darr || NULL == last_out, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(darr->elements < 1, DYN_ARR_INDEX_ERR);

    darr->elements--;
    *last_out = (byte *)darr->data + darr->elements * darr->elm_size;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_write_at_index(Dyn_arr * darr, int index, const void * key)
{
    sanity_chk(NULL == darr || NULL == key, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(index < 0 || index >= darr->elements, DYN_ARR_INDEX_ERR);

    darr->sorted = false;
    memcpy((byte *)darr->data + index * darr->elm_size,
           (byte *)key,
           darr->elm_size);
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_insert_at_index(Dyn_arr * darr, int index, const void * key)
{
    sanity_chk(NULL == darr || NULL == key, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(index < 0 || index >= darr->elements, DYN_ARR_INDEX_ERR);

    if (darr->elements == darr->capacity)
        if (dyn_arr_grow(darr) != 0)
            return DYN_ARR_MEM_ERR;

    memmove((byte *)darr->data + (index + 1) * darr->elm_size,
            (byte *)darr->data + index * darr->elm_size,
            (darr->elements - index) * darr->elm_size);

    dyn_arr_write_at_index(darr, index, key);
    // darr->sorted = false; not needed because dyn_arr_write_at_index()
    darr->elements++;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_remove_at_index(Dyn_arr * darr, int index)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(index < 0 || index >= darr->elements, DYN_ARR_INDEX_ERR);

    int last = darr->elements - 1;
    if (index < last)
        memmove((byte *)darr->data + index * darr->elm_size,
                (byte *)darr->data + (index + 1) * darr->elm_size,
                (last - index) * darr->elm_size);

    darr->elements--;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_remove_by_value(Dyn_arr * darr, const void * key)
{
    sanity_chk(NULL == darr || NULL == key || NULL == darr->compar,
                 DYN_ARR_NULL_PTR);

    const void * found;
    int ind = dyn_arr_const_find(darr, key, &found);

    if (ind != DYN_ARR_NOT_FOUND)
        return dyn_arr_remove_at_index(darr, ind);

    return DYN_ARR_NOT_FOUND;
}
//---------------------------------------------------------------------------

int dyn_arr_read_at_index(Dyn_arr * darr, int index, const void ** read_out)
{
    sanity_chk(NULL == darr || NULL == read_out, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(index < 0 || index >= darr->elements, DYN_ARR_INDEX_ERR);

    *read_out = (byte *)darr->data + index * darr->elm_size;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_read_back(Dyn_arr * darr, const void ** back_out)
{
    sanity_chk(NULL == darr || NULL == back_out, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(darr->elements <= 0, DYN_ARR_INDEX_ERR);

    *back_out = (byte *)darr->data + (darr->elements - 1) * darr->elm_size;
    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_sort(Dyn_arr * darr)
{
    sanity_chk(NULL == darr || NULL == darr->compar, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);

    if (false == darr->sorted)
    {
        int i; // check if it's already sorted
        for (i = 1; i < darr->elements; ++i)
        {
            if (darr->compar((byte *)darr->data + (i-1) * darr->elm_size,
                             (byte *)darr->data + i * darr->elm_size) > 0)
                       break;
        }

        if (i != darr->elements && darr->elements > 1)
            qsort(darr->data, darr->elements, darr->elm_size, darr->compar);

        darr->sorted = true;
    }

    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_const_find_from(Dyn_arr * darr, int index, const void * key, const void ** found)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);

    int sorted = darr->sorted;
    int ret = dyn_arr_find_from(darr, index, key, (void **)found);
    darr->sorted = sorted;
    return ret;
}

int dyn_arr_find_from(Dyn_arr * darr, int index, const void * key, void ** found)
{
    sanity_chk(NULL == darr || NULL == key || NULL == found || NULL == darr->compar,
                 DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(index < 0 || index >= darr->elements, DYN_ARR_INDEX_ERR);

    if (false == darr->sorted)
    {
        // linear search
        int i;
        byte * arr_key;
        for (i = index; i < darr->elements; ++i)
        {
            arr_key = (byte *)darr->data + i * darr->elm_size;
            if (darr->compar(arr_key, key) == 0)
            {
                *found = arr_key;
                return i;
            }
        }
        *found = NULL;
        return DYN_ARR_NOT_FOUND;
    }
    else
    {
        darr->sorted = false;
        // binary search
        int left, right, mid;
        left = index;
        right = darr->elements - 1;

        byte * arr_key;
        int compar_res;
        while (left <= right)
        {
            mid = left + ((right - left) / 2);
            arr_key = (byte *)darr->data + mid * darr->elm_size;
            compar_res = darr->compar(arr_key, key);

            if (compar_res < 0)
                left = mid + 1;
            else if (compar_res > 0)
                right = mid - 1;
            else
            {
                *found = arr_key;
                return mid;
            }
        }
        *found = NULL;
    }

    return DYN_ARR_NOT_FOUND;
}
//---------------------------------------------------------------------------

int dyn_arr_const_apply_to(Dyn_arr * darr, int elmnt_index,
                        void (*func)(const void * arr_elmnt, void * args), void * func_args)
{
    sanity_chk(NULL == darr, DYN_ARR_NULL_PTR);

    int sorted = darr->sorted;
    int ret = dyn_arr_apply_to(darr, elmnt_index, (void (*)(void *, void *))func, func_args);
    darr->sorted = sorted;
    return ret;
}

int dyn_arr_apply_to(Dyn_arr * darr, int elmnt_index,
                  void (*func)(void * arr_elmnt, void * args), void * func_args)
{
    sanity_chk(NULL == darr || NULL == func, DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data, DYN_ARR_BAD_ARRAY);
    sanity_chk(elmnt_index < DYN_ARR_APPLY_ALL || elmnt_index >= darr->elements,
                 DYN_ARR_INDEX_ERR);

    darr->sorted = false;
    int i;
    switch (elmnt_index)
    {
        case DYN_ARR_APPLY_ALL:
            for (i = 0; i < darr->elements; ++i)
                func((byte *)darr->data + i * darr->elm_size, func_args);
            break;

        default:
           func((byte *)darr->data + elmnt_index * darr->elm_size, func_args);
           break;
    }

    return 0;
}
//---------------------------------------------------------------------------

int dyn_arr_insert_online(Dyn_arr * darr, const void * key)
{
    sanity_chk(NULL == darr || NULL == key || NULL == darr->compar,
                 DYN_ARR_NULL_PTR);
    sanity_chk(NULL == darr->data || false == darr->sorted, DYN_ARR_BAD_ARRAY);

    int index, last = darr->elements - 1;
    if (darr->elements > 1)
    {
        if (darr->compar(key, (byte *)darr->data + last * darr->elm_size) >= 0)
        {
            index = last + 1;
            goto insert;
        }

        // modified binary search; looks for the proper place of key
        int left, right, mid;
        left = 0;
        right = last;

        byte * arr_key;
        int compar_res;
        while (left <= right)
        {
            mid = left + ((right - left) / 2);
            arr_key = (byte *)darr->data + mid * darr->elm_size;
            compar_res = darr->compar(arr_key, key);

            if (compar_res > 0)
                right = mid - 1;
            else
                left = mid + 1;
        }

        if (left < last)
            index = (darr->compar((byte *)darr->data + left * darr->elm_size, key) > 0) ? left : left + 1;
        else
            index = left;
    }
    else
        index = 0;

insert:
    if (darr->elements == darr->capacity)
        if (dyn_arr_grow(darr) != 0)
            return DYN_ARR_MEM_ERR;

    if (darr->elements > 0 && index <= last)
        memmove((byte *)darr->data + (index + 1) * darr->elm_size,
                (byte *)darr->data + index * darr->elm_size,
                (darr->elements - index) * darr->elm_size);

    memcpy((byte *)darr->data + index * darr->elm_size,
           (byte *)key,
           darr->elm_size);

    darr->elements++;
    return 0;
}
//---------------------------------------------------------------------------
