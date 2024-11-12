#ifndef PD_DARRAY_H
#define PD_DARRAY_H

#include "utils/defines.h"
#include "utils/types.h"

typedef struct pd_darray pd_darray;

struct pd_darray {
    void** data;
    usize size;
    usize cap;
};

typedef b32 (*pd_darray_is_equal_fn) (const void* _l, const void* _r);

PD_C_DECLS_BEGIN
b32 pd_darray_init(pd_darray* _arr, usize _cap);
void pd_darray_deinit(pd_darray* _arr);
b32 pd_darray_append(pd_darray* _arr, void* _data);
void* pd_darray_at(const pd_darray* _arr, usize _idx);
b32 pd_darray_contains_array(const pd_darray* _arr,
                             const pd_darray* _target,
                             pd_darray_is_equal_fn _cb);
void** pd_darray_r_data(const pd_darray* _arr);
usize pd_darray_r_size(const pd_darray* _arr);
usize pd_darray_r_cap(const pd_darray* _arr);
b32 pd_darray_is_empty(const pd_darray* _arr);
b32 pd_darray_is_full(const pd_darray* _arr);
PD_C_DECLS_END

#endif // PD_DARRAY_H
