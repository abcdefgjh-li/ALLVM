#define REF_FN __attribute__((noinline))

extern "C" REF_FN int target_call(int x) {
    return x + 7;
}

extern "C" REF_FN int call_ret_probe(int x) {
    return target_call(x) + 1;
}
