extern "C" __attribute__((noinline)) int frame_record_callee(int x) {
    return x + 7;
}

extern "C" __attribute__((noinline)) int frame_record_probe(int x) {
    int y = frame_record_callee(x);
    return (y ^ 0x5a) - 0x10;
}
