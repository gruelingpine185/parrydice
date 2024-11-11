#include <stdio.h>

#include "utils/expect.h"

static const char* const expect_op_list[] = {
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">="
};

const char* pd_expect_op_as_str(pd_expect_op _op) {
    return expect_op_list[_op];
}

void pd_expect(const char* _pathname, i32 _ln, pd_expect_ctx* _ctx) {
    fprintf(
        stderr,
        "%s:%d: Assertion Failed\n  Expected: (_l %s _r)\n",
         _pathname,
        _ln,
        pd_expect_op_as_str(_ctx->op));
    if(_ctx->type == PD_EXPECT_TYPE_PTR) {
        fprintf(
            stderr,
            "  _l: %s (%p)\n"
            "  _r: %s (%p)\n",
            _ctx->l_as_str,
            _ctx->value.as_ptr.l,
            _ctx->r_as_str,
            _ctx->value.as_ptr.r);
    } else {
        fprintf(
            stderr,
            "  _l: %s (%d)\n"
            "  _r: %s (%d)\n",
            _ctx->l_as_str,
            _ctx->value.as_expr.l,
            _ctx->r_as_str,
            _ctx->value.as_expr.r);
    }
    exit(1);
}
