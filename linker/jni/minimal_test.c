/*
 * 极简测试程序 - 直接使用内联汇编
 */

// 系统调用包装函数
static inline long syscall1(long number, long arg1) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = arg1;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
    return x0;
}

static inline long syscall3(long number, long arg1, long arg2, long arg3) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = arg1;
    register long x1 __asm__("x1") = arg2;
    register long x2 __asm__("x2") = arg3;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
    return x0;
}

// write 系统调用
static void my_write(int fd, const char* buf, unsigned long count) {
    syscall3(64, fd, (long)buf, count);
}

// exit 系统调用
static void my_exit(int code) {
    syscall1(93, code);
    __builtin_unreachable();
}

// 字符串长度
static unsigned long my_strlen(const char* s) {
    unsigned long len = 0;
    while (s[len]) len++;
    return len;
}

// 打印字符串
static void print(const char* s) {
    my_write(1, s, my_strlen(s));
}

// 全局变量
int g_global = 42;

// 简单函数
int add(int a, int b) {
    return a + b;
}

// 主函数
void _start() {
    print("========================================\n");
    print("  Minimal Test Program\n");
    print("========================================\n");
    
    print("Global var: 42\n");
    
    int result = add(10, 20);
    if (result == 30) {
        print("add(10, 20) = 30 [OK]\n");
    }
    
    print("========================================\n");
    print("  Test Completed!\n");
    print("========================================\n");
    
    my_exit(0);
}
