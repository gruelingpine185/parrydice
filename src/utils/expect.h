#ifndef PD_ASSERT_H
#define PD_ASSERT_H

#include "utils/defines.h"
#include "utils/types.h"

#define PD_expect_ptr(_l, _op, _r) \
    do { \
        const char* null_str = "NULL"; \
        pd_expect_ctx ctx = {0}; \
        ctx.type = PD_EXPECT_TYPE_PTR; \
        ctx.op = _op; \
        ctx.l_as_str = (!_l)? \
            null_str: PD_stringify(_l); \
        ctx.r_as_str = (!_r)? \
            null_str: PD_stringify(_r); \
        ctx.value.as_ptr.l = _l; \
        ctx.value.as_ptr.r = _r; \
        pd_expect(__FILE__, __LINE__, &ctx); \
    } while(0)

#define PD_expect_null(_l) \
    do { \
        if((_l)) PD_expect_ptr(_l, PD_EXPECT_OP_EQ, NULL); \
     } while(0)

#define PD_expect_nonnull(_l) \
    do { \
        if(!(_l)) PD_expect_ptr(_l, PD_EXPECT_OP_NE, NULL); \
     } while(0)

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
    PD_EXPECT_OP_GTE
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
void pd_expect(const char* _pathname, i32 _ln, pd_expect_ctx* _ctx);
PD_C_DECLS_END

#endif //SERT_H
