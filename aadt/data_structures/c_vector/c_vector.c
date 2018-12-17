#include <string.h>
#include <stdlib.h>
#include "c_vector.h"

typedef unsigned char byte;

#define grow(cv) do {\
    if (cv->elem_count >= cv->capacity &&\
		!c_vect_resize(cv, cv->capacity*C_VECT_GROWTH_RATE))\
		return NULL;\
} while (0)
//------------------------------------------------------------------------------

void * c_vect_resize(c_vector * cv, int new_capacity)
{
    void * result = NULL;

    if (new_capacity > 0)
    {
        result = realloc(cv->arr, new_capacity*cv->elem_size);
        if (result)
        {
            cv->arr = result;
            cv->capacity = new_capacity;
            if (cv->elem_count > new_capacity)
                cv->elem_count = new_capacity;
            result = cv;
        }
    }

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_make_cap(c_vector * cv, int elem_size, fcomp compar, int capacity)
{
    cv->elem_size = elem_size;
    cv->capacity = capacity;
    cv->elem_count = 0;
    cv->arr = NULL;
    cv->compar = compar;

    void * vect = NULL;

     if ((elem_size <= 0) || !(vect = c_vect_resize(cv, capacity)))
        memset(cv, 0, sizeof(*cv));

    return vect;
}
//------------------------------------------------------------------------------

void c_vect_destroy(c_vector * cv)
{
    free(cv->arr);
    memset(cv, 0, sizeof(*cv));
}
//------------------------------------------------------------------------------

void * c_vect_push(c_vector * cv, const void * what)
{
    grow(cv);

    void * dest = (byte *)cv->arr + cv->elem_count * cv->elem_size;
    ++cv->elem_count;

    return memcpy(dest, what, cv->elem_size);
}
//------------------------------------------------------------------------------

void * c_vect_pop(c_vector * cv)
{
    void * ret = NULL;

    if (cv->elem_count > 0)
    {
        --cv->elem_count;
        ret = cv;
    }

    return ret;
}
//------------------------------------------------------------------------------

void * c_vect_peek(c_vector * cv)
{
    void * result = NULL;

    if (cv->elem_count > 0)
        result = (byte *)cv->arr + (cv->elem_count - 1) * cv->elem_size;

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_trim(c_vector * cv)
{
    if (cv->elem_count == cv->capacity)
        return cv;

    return c_vect_resize(cv, cv->elem_count);
}

//------------------------------------------------------------------------------

void * c_vect_find_ind(c_vector * cv, const void * key, int * out_index)
{
    byte * cv_key = NULL, * data = (byte *)cv->arr;
    int elem_size = cv->elem_size, elem_count = cv->elem_count;
    fcomp compar = cv->compar;

    if (out_index)
        *out_index = -1;

    void * found = NULL;
    for(int i = 0; i < elem_count; ++i)
    {
        cv_key = data + i * elem_size;
        if (compar(cv_key, key) == 0)
        {
            found = cv_key;
            if (out_index)
                *out_index = i;
            break;
        }
    }

    return found;
}
//------------------------------------------------------------------------------

void * c_vect_is_sorted(c_vector * cv)
{
    byte * data = (byte *)cv->arr;
    int elem_size = cv->elem_size, elem_count = cv->elem_count;
    fcomp compar = cv->compar;

    void * result = elem_count ? cv : NULL;
    if (elem_count > 1)
    {
        for (int i = 1, prev = i - 1; i < elem_count; ++i, ++prev)
        {
            if (compar(data + prev * elem_size, data + i * elem_size) > 0)
            {
                result = NULL;
                break;
            }
        }
    }

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_sort(c_vector * cv)
{
    if (!c_vect_is_sorted(cv))
        qsort(cv->arr, cv->elem_count, cv->elem_size, cv->compar);

    return cv;
}
//------------------------------------------------------------------------------

void * c_vect_bsearch_ind(c_vector * cv, const void * key, int * out_index)
{
    byte * data = (byte *)cv->arr, * cv_key = NULL;
    int elem_size = cv->elem_size, elem_count = cv->elem_count;
    fcomp compar = cv->compar;

    if (out_index)
        *out_index = -1;

    void * result = NULL;
    if (elem_count > 0)
    {
        int left = 0, right = elem_count - 1, mid, comp;
        while (left <= right)
        {
            mid = left + ((right - left) / 2);
            cv_key = data + mid * elem_size;

            comp = compar(cv_key, key);
            if (comp < 0)
                left = mid + 1;
            else if (comp > 0)
                right = mid - 1;
            else
            {
                result = cv_key;
                if (out_index)
                    *out_index = mid;
                break;
            }
        }
    }

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_insert_online_ind(
    c_vector * cv, const void * key, int * out_index
    )
{
    byte * data = (byte *)cv->arr;
    fcomp compar = cv->compar;
    int elem_count = cv->elem_count, elem_size = cv->elem_size;

    int index, last = elem_count - 1;
    if (elem_count > 0)
    {
        if (compar(key, data + last * elem_size) >= 0)
        {
            index = last + 1;
            goto insert;
        }

        byte * cv_key = NULL;
        int left = 0, right = last, mid;
        while (left <= right)
        {
            mid = left + ((right - left) / 2);
            cv_key = data + mid * elem_size;

            if (compar(cv_key, key) > 0)
                right = mid - 1;
            else
                left = mid + 1;
        }

        if (left < last)
            index =
            (compar(data + left * elem_size, key) > 0) ? left : left + 1;
        else
            index = left;
    }
    else
        index = 0;

insert:
    grow(cv);
    data = (byte *)cv->arr;

    if (elem_count > 0 && index <= last)
        memmove(data + (index + 1) * elem_size,
                data + index * elem_size,
                (elem_count - index) * elem_size);

    void * dest = data + index * elem_size;
    memcpy(dest, key, elem_size);

    if (out_index)
        *out_index = index;

    ++cv->elem_count;
    return dest;
}
//------------------------------------------------------------------------------

void * c_vect_append_array(c_vector * cv, const void * arr, int len)
{
    void * result = cv;
    while (cv->capacity < cv->elem_count + len)
    {
        if (!c_vect_resize(cv, cv->capacity*C_VECT_GROWTH_RATE))
            return NULL;
    }

    memcpy((byte *)cv->arr + cv->elem_count * cv->elem_size,
            arr,
            cv->elem_size * len);

    cv->elem_count += len;
    return result;
}
//------------------------------------------------------------------------------

void * c_vect_get(c_vector * cv, int index)
{
    void * result = NULL;

    if (index >= 0 && index < cv->elem_count)
        result = (byte *)cv->arr + index * cv->elem_size;

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_write_at_index(c_vector * cv, int index, const void * key)
{
    void * result = NULL;

    if (index >= 0 && index < cv->elem_count)
    {
        int elem_size = cv->elem_size;
        result = memcpy((byte *)cv->arr + index * elem_size, key, elem_size);
    }

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_insert_at_index(c_vector * cv, int index, const void * key)
{
    void * result = NULL;

    if (index >= 0 && index < cv->elem_count)
    {
        grow(cv);
        byte * data = (byte *)cv->arr;
        int elem_size = cv->elem_size;
        void * source = data + index * elem_size;

        memmove(data + (index + 1) * elem_size,
                source,
                (cv->elem_count - index) * elem_size);

        c_vect_write_at_index(cv, index, key);
        ++cv->elem_count;
        result = source;
    }

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_remove_at_index(c_vector * cv, int index)
{
    void * result = NULL;

    if (index >= 0 && index < cv->elem_count)
    {
        byte * data = (byte *)cv->arr;
        int last = cv->elem_count - 1, elem_size = cv->elem_size;

        if (index < last)
            memmove(data + index * elem_size,
                    data + (index + 1) * elem_size,
                    (last - index) * elem_size);

        --cv->elem_count;
        result = cv;
    }

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_apply_args(c_vector * cv, fapply_args fun, void * args)
{
    byte * data = (byte *)cv->arr;
    int all = cv->elem_count, elem_size = cv->elem_size;

    for (int i = 0; i < all; ++i)
        fun(data + i * elem_size, args);

    return cv;
}
//------------------------------------------------------------------------------

void * c_vect_apply(c_vector * cv, fapply fun)
{
    byte * data = (byte *)cv->arr;
    int all = cv->elem_count, elem_size = cv->elem_size;

    for (int i = 0; i < all; ++i)
        fun(data + i * elem_size);

    return cv;
}
//------------------------------------------------------------------------------

void * c_vect_zero_out(c_vector * cv)
{
    memset(cv->arr, 0, cv->capacity * cv->elem_size);
    return cv;
}
//------------------------------------------------------------------------------

void * c_vect_set_length(c_vector * cv, int len)
{
    void * result = NULL;

    if (len >= 0 && len <= cv->capacity)
    {
        cv->elem_count = len;
        result = cv;
    }

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_copy(c_vector * dest, const c_vector * src)
{
    void * ret = NULL;

    int dsize = src->capacity * src->elem_size;
    void * new_data = malloc(dsize);
    if (new_data)
    {
        memcpy(new_data, src->arr, dsize);
        *dest = *src;
        dest->arr = new_data;
        ret = dest;
    }

    return ret;
}
//------------------------------------------------------------------------------

void * c_vect_reset(c_vector * cv)
{
    cv->elem_count = 0;
    return cv;
}
//------------------------------------------------------------------------------

void * c_vect_set_compar(c_vector * cv, fcomp compar)
{
    cv->compar = compar;
    return cv;
}
//------------------------------------------------------------------------------

bool c_vect_is_empty(c_vector * cv)
{
    return !cv->elem_count;
}
//------------------------------------------------------------------------------

void * c_vect_data(c_vector * cv)
{
    return cv->arr;
}
//------------------------------------------------------------------------------

int c_vect_length(c_vector * cv)
{
    return cv->elem_count;
}
//------------------------------------------------------------------------------

int c_vect_elem_size(c_vector * cv)
{
    return cv->elem_size;
}
//------------------------------------------------------------------------------

int c_vect_capacity(c_vector * cv)
{
    return cv->capacity;
}
//------------------------------------------------------------------------------
