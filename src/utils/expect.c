#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <execinfo.h>

#include "utils/expect.h"

static const char* const expect_op_list[] = {
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">="
};

static void pd_display_callstack(void);


static void pd_display_callstack(void) {
    const u32 max_frames = 16;
    void* array[max_frames];
    char** strings = NULL;
    int size, i;

    size = backtrace(array, max_frames);
    strings = backtrace_symbols(array, size);
    if(!strings) return;

    // discards call to this function
    for(i = 1; i < size; i++) {
        printf("frame: #%s\n", strings[i]);
    }

    free(strings);
}

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
    const i32 l_str_len = (i32) strlen(_ctx->l_as_str);
    const i32 r_str_len = (i32) strlen(_ctx->r_as_str);
    const i32 max_len = (l_str_len >= r_str_len)?
        l_str_len: r_str_len;

    if(_ctx->type == PD_EXPECT_TYPE_PTR) {
        fprintf(
            stderr,
            "  _l: %-*s (%p)\n"
            "  _r: %-*s (%p)\n",
            max_len,
            _ctx->l_as_str,
            _ctx->value.as_ptr.l,
            max_len,
            _ctx->r_as_str,
            _ctx->value.as_ptr.r);
    } else {
        fprintf(
            stderr,
            "  _l: %-*s (%d)\n"
            "  _r: %-*s (%d)\n",
            max_len,
            _ctx->l_as_str,
            _ctx->value.as_expr.l,
            max_len,
            _ctx->r_as_str,
            _ctx->value.as_expr.r);
    }

    printf("\n");
    pd_display_callstack();
    exit(1);
}
