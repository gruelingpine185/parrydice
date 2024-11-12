#include <stdlib.h>

#include "utils/expect.h"
#include "utils/darray.h"

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
