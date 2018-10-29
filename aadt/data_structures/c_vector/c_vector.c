/* c_vector.c -- a dynamic array implementation */
/* v1.0 */
#include <string.h>
#include <stdlib.h>
#include "c_vector.h"

typedef unsigned char byte;
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

void * c_vect_make_cap(c_vector * cv, int elem_size, int capacity)
{
    cv->elem_size = elem_size;
    cv->capacity = capacity;
    cv->elem_count = 0;
    cv->arr = NULL;

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

void * c_vect_push(c_vector * cv, void * what)
{
    if (cv->elem_count >= cv->capacity &&
        !c_vect_resize(cv, cv->capacity*C_VECT_GROWTH_RATE))
        return NULL;

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
        result = (byte *)cv->arr + (cv->elem_count-1) * cv->elem_size;

    return result;
}
//------------------------------------------------------------------------------

void * c_vect_get(c_vector * cv, int ind)
{
    void * result = NULL;

    if (ind >= 0 && ind < cv->elem_count)
        result = (byte *)cv->arr + ind * cv->elem_size;

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
