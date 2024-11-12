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

b32 pd_darray_append(pd_darray* _arr, void* _data) {
    PD_expect_nonnull(_arr);
    PD_expect_nonnull(_arr->data);
    PD_expect_nonnull(_data);
    if(!pd_darray_r_cap(_arr)) {
        if(!pd_darray_resize(_arr)) return 0;
    } else if(pd_darray_r_size(_arr) < pd_darray_r_cap(_arr)) {
        _arr->data[_arr->size++] = _data;
        return 1;
    }

    if(!pd_darray_resize(_arr)) return 0;

    _arr->data[++_arr->size] = _data;
    return 1;
}

void* pd_darray_at(const pd_darray* _arr, usize _idx) {
    PD_expect_nonnull(_arr);
    PD_expect_nonnull(_arr->data);
    PD_expect_expr_gte(_idx, 0);
    PD_expect_expr_lt(_idx, pd_darray_r_cap(_arr));
    return _arr->data[_idx];
}

b32 pd_darray_is_empty(const pd_darray* _arr) {
    PD_expect_nonnull(_arr);
    return (pd_darray_r_size(_arr) == 0);
}

b32 pd_darray_is_full(const pd_darray* _arr) {
    return (pd_darray_r_size(_arr) == pd_darray_r_cap(_arr));
}

void** pd_darray_r_data(const pd_darray* _arr) {
    PD_expect_nonnull(_arr);
    return _arr->data;
}

usize pd_darray_r_size(const pd_darray* _arr) {
    PD_expect_nonnull(_arr);
    return _arr->size;
}

usize pd_darray_r_cap(const pd_darray* _arr) {
    PD_expect_nonnull(_arr);
    return _arr->cap;
}
