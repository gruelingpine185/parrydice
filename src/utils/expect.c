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
