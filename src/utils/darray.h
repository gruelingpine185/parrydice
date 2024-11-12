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

PD_C_DECLS_BEGIN
b32 pd_darray_init(pd_darray* _arr);
void pd_darray_deinit(pd_darray* _arr);
PD_C_DECLS_END

#endif // PD_DARRAY_H
