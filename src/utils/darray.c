#include <stdlib.h>

#include "utils/expect.h"
#include "utils/darray.h"

static b32 pd_darray_resize(pd_darray* _arr);


static b32 pd_darray_resize(pd_darray* _arr) {
    const f32 resize_factor = 1.61f;
    const usize new_cap = pd_darray_r_cap(_arr) * resize_factor;
    void** tmp = (void**) realloc(&_arr->data, (sizeof(void*) * new_cap));
    if(!tmp) return 0;

    _arr->data = tmp;
    _arr->cap = new_cap;
    return 1;
}

b32 pd_darray_init(pd_darray* _arr) {
    PD_expect_nonnull(_arr);

    _arr->cap = 16;
    _arr->data = (void**) malloc(sizeof(void*) * _arr->cap);
    if(!_arr->data) return 0;

    _arr->size = 0;
    return 1;
}

void pd_darray_deinit(pd_darray* _arr) {
    PD_expect_nonnull(_arr);
    PD_expect_nonnull(_arr->data);
    free(_arr->data);
}
