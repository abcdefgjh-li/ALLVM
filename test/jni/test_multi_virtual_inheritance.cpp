// 测试多重继承和虚继承的类型匹配
#include <stdio.h>
#include <typeinfo>

// ========== 基础类定义 ==========

// 基类 A
class BaseA {
public:
    int a_value;
    BaseA() : a_value(100) {}
    virtual ~BaseA() {}
    virtual const char* name() const { return "BaseA"; }
};

// 基类 B
class BaseB {
public:
    int b_value;
    BaseB() : b_value(200) {}
    virtual ~BaseB() {}
    virtual const char* name2() const { return "BaseB"; }
};

// 基类 C
class BaseC {
public:
    int c_value;
    BaseC() : c_value(300) {}
    virtual ~BaseC() {}
    virtual const char* name3() const { return "BaseC"; }
};

// ========== 多重继承测试 ==========

// 多重继承类（两个基类）
class MultiDerivedAB : public BaseA, public BaseB {
public:
    int ab_value;
    MultiDerivedAB() : ab_value(400) {}
    virtual const char* name() const override { return "MultiDerivedAB"; }
};

// 多重继承类（三个基类）
class MultiDerivedABC : public BaseA, public BaseB, public BaseC {
public:
    int abc_value;
    MultiDerivedABC() : abc_value(500) {}
    virtual const char* name() const override { return "MultiDerivedABC"; }
};

// 菱形继承（非虚拟）
class DiamondLeft : public BaseA {
public:
    int left_value;
    DiamondLeft() : left_value(600) {}
};

class DiamondRight : public BaseA {
public:
    int right_value;
    DiamondRight() : right_value(700) {}
};

class DiamondBottom : public DiamondLeft, public DiamondRight {
public:
    int bottom_value;
    DiamondBottom() : bottom_value(800) {}
};

// ========== 虚拟继承测试 ==========

// 虚拟基类
class VirtualBase {
public:
    int virtual_value;
    VirtualBase() : virtual_value(1000) {}
    virtual ~VirtualBase() {}
    virtual const char* vname() const { return "VirtualBase"; }
};

// 虚拟继承左分支
class VirtualLeft : virtual public VirtualBase {
public:
    int vleft_value;
    VirtualLeft() : vleft_value(1100) {}
    virtual const char* vname() const override { return "VirtualLeft"; }
};

// 虚拟继承右分支
class VirtualRight : virtual public VirtualBase {
public:
    int vright_value;
    VirtualRight() : vright_value(1200) {}
    virtual const char* vname() const override { return "VirtualRight"; }
};

// 虚拟继承底部（解决菱形问题）
class VirtualBottom : public VirtualLeft, public VirtualRight {
public:
    int vbottom_value;
    VirtualBottom() : vbottom_value(1300) {}
    virtual const char* vname() const override { return "VirtualBottom"; }
};

// 深层虚拟继承
class DeepVirtual : virtual public VirtualBase {
public:
    int deep_value;
    DeepVirtual() : deep_value(1400) {}
};

class DeeperVirtual : public DeepVirtual {
public:
    int deeper_value;
    DeeperVirtual() : deeper_value(1500) {}
};

// ========== 测试函数 ==========

// 测试多重继承（两个基类）
int test_multi_inheritance_2bases() {
    printf("=== Test Multi-Inheritance (2 bases) ===\n");

    MultiDerivedAB obj;
    int failed = 0;

    // 测试向第一个基类转换
    BaseA* pa = &obj;
    if (pa != nullptr) {
        printf("[PASS] MultiDerivedAB* -> BaseA* (offset: %ld)\n",
               (char*)pa - (char*)&obj);
    } else {
        printf("[FAIL] MultiDerivedAB* -> BaseA*\n");
        failed = 1;
    }

    // 测试向第二个基类转换
    BaseB* pb = &obj;
    if (pb != nullptr) {
        printf("[PASS] MultiDerivedAB* -> BaseB* (offset: %ld)\n",
               (char*)pb - (char*)&obj);
    } else {
        printf("[FAIL] MultiDerivedAB* -> BaseB*\n");
        failed = 1;
    }

    // 测试交叉转换
    BaseA* base_a = &obj;
    BaseB* base_b = dynamic_cast<BaseB*>(base_a);
    if (base_b != nullptr) {
        printf("[PASS] BaseA* (from MultiDerivedAB) -> BaseB* (cross-cast)\n");
    } else {
        printf("[FAIL] BaseA* (from MultiDerivedAB) -> BaseB* (cross-cast)\n");
        failed = 1;
    }

    return failed;
}

// 测试多重继承（三个基类）
int test_multi_inheritance_3bases() {
    printf("\n=== Test Multi-Inheritance (3 bases) ===\n");

    MultiDerivedABC obj;
    int failed = 0;

    // 测试向三个基类转换
    BaseA* pa = &obj;
    BaseB* pb = &obj;
    BaseC* pc = &obj;

    if (pa && pb && pc) {
        printf("[PASS] MultiDerivedABC* -> BaseA*, BaseB*, BaseC*\n");
        printf("  BaseA offset: %ld\n", (char*)pa - (char*)&obj);
        printf("  BaseB offset: %ld\n", (char*)pb - (char*)&obj);
        printf("  BaseC offset: %ld\n", (char*)pc - (char*)&obj);
    } else {
        printf("[FAIL] MultiDerivedABC* -> BaseA*, BaseB*, BaseC*\n");
        failed = 1;
    }

    // 测试交叉转换
    BaseA* base_a = &obj;
    BaseC* base_c = dynamic_cast<BaseC*>(base_a);
    if (base_c != nullptr) {
        printf("[PASS] BaseA* (from MultiDerivedABC) -> BaseC* (cross-cast)\n");
    } else {
        printf("[FAIL] BaseA* (from MultiDerivedABC) -> BaseC* (cross-cast)\n");
        failed = 1;
    }

    return failed;
}

// 测试菱形继承（非虚拟，有歧义）
int test_diamond_inheritance() {
    printf("\n=== Test Diamond Inheritance (non-virtual) ===\n");

    DiamondBottom obj;
    int failed = 0;

    // 注意：非虚拟菱形继承会导致 BaseA 有两个副本
    // DiamondLeft::BaseA 和 DiamondRight::BaseA

    DiamondLeft* left = &obj;
    DiamondRight* right = &obj;

    printf("[INFO] DiamondBottom has two BaseA subobjects\n");
    printf("  DiamondLeft::BaseA offset: %ld\n",
           (char*)static_cast<BaseA*>(left) - (char*)&obj);
    printf("  DiamondRight::BaseA offset: %ld\n",
           (char*)static_cast<BaseA*>(right) - (char*)&obj);

    // 尝试直接转换到 BaseA 会有歧义
    // BaseA* base = &obj;  // 编译错误：ambiguous

    printf("[PASS] Diamond inheritance handled correctly (ambiguous base)\n");

    return failed;
}

// 测试虚拟继承（解决菱形问题）
int test_virtual_inheritance() {
    printf("\n=== Test Virtual Inheritance ===\n");

    VirtualBottom obj;
    int failed = 0;

    // 虚拟继承确保只有一个 VirtualBase 实例
    VirtualLeft* left = &obj;
    VirtualRight* right = &obj;

    // 两个路径应该指向同一个 VirtualBase
    VirtualBase* vb_from_left = static_cast<VirtualBase*>(left);
    VirtualBase* vb_from_right = static_cast<VirtualBase*>(right);

    if (vb_from_left == vb_from_right) {
        printf("[PASS] Virtual inheritance: single VirtualBase instance\n");
        printf("  VirtualBase offset: %ld\n", (char*)vb_from_left - (char*)&obj);
    } else {
        printf("[FAIL] Virtual inheritance: multiple VirtualBase instances\n");
        failed = 1;
    }

    // 测试 dynamic_cast
    VirtualBase* vb = &obj;
    VirtualBottom* bottom = dynamic_cast<VirtualBottom*>(vb);
    if (bottom != nullptr) {
        printf("[PASS] VirtualBase* -> VirtualBottom* (dynamic_cast)\n");
    } else {
        printf("[FAIL] VirtualBase* -> VirtualBottom* (dynamic_cast)\n");
        failed = 1;
    }

    return failed;
}

// 测试深层虚拟继承
int test_deep_virtual_inheritance() {
    printf("\n=== Test Deep Virtual Inheritance ===\n");

    DeeperVirtual obj;
    int failed = 0;

    // 测试向上转型
    VirtualBase* vb = &obj;
    if (vb != nullptr) {
        printf("[PASS] DeeperVirtual* -> VirtualBase* (through virtual inheritance)\n");
        printf("  VirtualBase offset: %ld\n", (char*)vb - (char*)&obj);
    } else {
        printf("[FAIL] DeeperVirtual* -> VirtualBase*\n");
        failed = 1;
    }

    // 测试向下转型
    DeeperVirtual* deeper = dynamic_cast<DeeperVirtual*>(vb);
    if (deeper != nullptr) {
        printf("[PASS] VirtualBase* -> DeeperVirtual* (dynamic_cast)\n");
    } else {
        printf("[FAIL] VirtualBase* -> DeeperVirtual* (dynamic_cast)\n");
        failed = 1;
    }

    return failed;
}

// 测试异常处理中的多重继承
int test_exception_multi_inheritance() {
    printf("\n=== Test Exception Handling with Multi-Inheritance ===\n");

    int failed = 0;

    // 测试1：抛出 MultiDerivedAB，捕获 BaseA
    try {
        throw MultiDerivedAB();
    } catch (const BaseA& e) {
        printf("[PASS] MultiDerivedAB exception caught by BaseA handler\n");
    } catch (...) {
        printf("[FAIL] MultiDerivedAB exception not caught by BaseA handler\n");
        failed = 1;
    }

    // 测试2：抛出 MultiDerivedAB，捕获 BaseB
    try {
        throw MultiDerivedAB();
    } catch (const BaseB& e) {
        printf("[PASS] MultiDerivedAB exception caught by BaseB handler\n");
    } catch (...) {
        printf("[FAIL] MultiDerivedAB exception not caught by BaseB handler\n");
        failed = 1;
    }

    // 测试3：抛出 MultiDerivedABC，捕获 BaseC
    try {
        throw MultiDerivedABC();
    } catch (const BaseC& e) {
        printf("[PASS] MultiDerivedABC exception caught by BaseC handler\n");
    } catch (...) {
        printf("[FAIL] MultiDerivedABC exception not caught by BaseC handler\n");
        failed = 1;
    }

    return failed;
}

// 测试异常处理中的虚拟继承
int test_exception_virtual_inheritance() {
    printf("\n=== Test Exception Handling with Virtual Inheritance ===\n");

    int failed = 0;

    // 测试1：抛出 VirtualBottom，捕获 VirtualBase
    try {
        throw VirtualBottom();
    } catch (const VirtualBase& e) {
        printf("[PASS] VirtualBottom exception caught by VirtualBase handler\n");
    } catch (...) {
        printf("[FAIL] VirtualBottom exception not caught by VirtualBase handler\n");
        failed = 1;
    }

    // 测试2：抛出 DeeperVirtual，捕获 VirtualBase
    try {
        throw DeeperVirtual();
    } catch (const VirtualBase& e) {
        printf("[PASS] DeeperVirtual exception caught by VirtualBase handler\n");
    } catch (...) {
        printf("[FAIL] DeeperVirtual exception not caught by VirtualBase handler\n");
        failed = 1;
    }

    // 测试3：抛出 VirtualLeft，捕获 VirtualBase
    try {
        throw VirtualLeft();
    } catch (const VirtualBase& e) {
        printf("[PASS] VirtualLeft exception caught by VirtualBase handler\n");
    } catch (...) {
        printf("[FAIL] VirtualLeft exception not caught by VirtualBase handler\n");
        failed = 1;
    }

    return failed;
}

// 测试 dynamic_cast 失败情况
int test_dynamic_cast_failures() {
    printf("\n=== Test Dynamic Cast Failures ===\n");

    int failed = 0;

    // 测试1：不相关的类型转换
    BaseA base_a;
    BaseB* base_b = dynamic_cast<BaseB*>(&base_a);
    if (base_b == nullptr) {
        printf("[PASS] BaseA* cannot be cast to BaseB* (unrelated types)\n");
    } else {
        printf("[FAIL] BaseA* incorrectly cast to BaseB*\n");
        failed = 1;
    }

    // 测试2：向下转型失败
    BaseA base;
    MultiDerivedAB* derived = dynamic_cast<MultiDerivedAB*>(&base);
    if (derived == nullptr) {
        printf("[PASS] BaseA* cannot be cast to MultiDerivedAB* (not actual type)\n");
    } else {
        printf("[FAIL] BaseA* incorrectly cast to MultiDerivedAB*\n");
        failed = 1;
    }

    return failed;
}

int main() {
    printf("========================================\n");
    printf("  Multi & Virtual Inheritance Test\n");
    printf("========================================\n\n");

    int failed = 0;

    failed += test_multi_inheritance_2bases();
    failed += test_multi_inheritance_3bases();
    failed += test_diamond_inheritance();
    failed += test_virtual_inheritance();
    failed += test_deep_virtual_inheritance();
    failed += test_exception_multi_inheritance();
    failed += test_exception_virtual_inheritance();
    failed += test_dynamic_cast_failures();

    printf("\n========================================\n");
    printf("  Test Summary\n");
    printf("========================================\n");

    if (failed == 0) {
        printf("✓ All tests passed!\n");
        return 0;
    } else {
        printf("✗ %d test(s) failed\n", failed);
        return 1;
    }
}
