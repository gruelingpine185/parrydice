#ifndef PD_ASSERT_H
#define PD_ASSERT_H

#include "utils/defines.h"
#include "utils/types.h"

typedef enum {
    PD_EXPECT_TYPE_PTR,
    PD_EXPECT_TYPE_EXPR
} pd_expect_type;

typedef enum {
    PD_EXPECT_OP_EQ,
    PD_EXPECT_OP_NE,
    PD_EXPECT_OP_LT,
    PD_EXPECT_OP_GT,
    PD_EXPECT_OP_LTE,
    PD_ASSRET_OP_GTE
} pd_expect_op;

typedef struct pd_expect_ctx pd_expect_ctx;
typedef union pd_expect_value pd_expect_value;

union pd_expect_value {
    struct {
        void* l;
        void* r;
    } as_ptr;

    struct {
        i32 l;
        i32 r;
    } as_expr;
};

struct pd_expect_ctx {
    pd_expect_type type;
    pd_expect_op op;
    const char* l_as_str;
    const char* r_as_str;
    pd_expect_value value;
};

PD_C_DECLS_BEGIN
const char* pd_expect_op_as_str(pd_expect_op _op);
PD_C_DECLS_END

#endif //SERT_H

