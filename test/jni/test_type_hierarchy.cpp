// 测试完整的类型层次遍历和动态类型信息解析
#include <stdio.h>
#include <typeinfo>
#include <exception>

// ========== 深层继承层次 ==========

// 第1层：基础类
class Level1 {
public:
    int l1_value;
    Level1() : l1_value(100) {}
    virtual ~Level1() {}
    virtual const char* name() const { return "Level1"; }
};

// 第2层：单继承
class Level2 : public Level1 {
public:
    int l2_value;
    Level2() : l2_value(200) {}
    virtual const char* name() const override { return "Level2"; }
};

// 第3层：继续单继承
class Level3 : public Level2 {
public:
    int l3_value;
    Level3() : l3_value(300) {}
    virtual const char* name() const override { return "Level3"; }
};

// 第4层：继续单继承
class Level4 : public Level3 {
public:
    int l4_value;
    Level4() : l4_value(400) {}
    virtual const char* name() const override { return "Level4"; }
};

// 第5层：继续单继承
class Level5 : public Level4 {
public:
    int l5_value;
    Level5() : l5_value(500) {}
    virtual const char* name() const override { return "Level5"; }
};

// ========== 复杂多重继承 ==========

// 基类 A
class BaseA {
public:
    int a_value;
    BaseA() : a_value(1000) {}
    virtual ~BaseA() {}
    virtual const char* name() const { return "BaseA"; }
};

// 基类 B
class BaseB {
public:
    int b_value;
    BaseB() : b_value(2000) {}
    virtual ~BaseB() {}
    virtual const char* name2() const { return "BaseB"; }
};

// 中间层：多重继承
class MiddleAB : public BaseA, public BaseB {
public:
    int ab_value;
    MiddleAB() : ab_value(3000) {}
    virtual const char* name() const override { return "MiddleAB"; }
};

// 派生层：继续继承
class DerivedAB : public MiddleAB {
public:
    int derived_value;
    DerivedAB() : derived_value(4000) {}
    virtual const char* name() const override { return "DerivedAB"; }
};

// ========== 虚拟继承深层层次 ==========

// 虚拟基类
class VirtualBase {
public:
    int vbase_value;
    VirtualBase() : vbase_value(10000) {}
    virtual ~VirtualBase() {}
    virtual const char* vname() const { return "VirtualBase"; }
};

// 虚拟继承层1
class VLevel1 : virtual public VirtualBase {
public:
    int vl1_value;
    VLevel1() : vl1_value(11000) {}
    virtual const char* vname() const override { return "VLevel1"; }
};

// 虚拟继承层2
class VLevel2 : virtual public VirtualBase {
public:
    int vl2_value;
    VLevel2() : vl2_value(12000) {}
    virtual const char* vname() const override { return "VLevel2"; }
};

// 虚拟继承层3（菱形底部）
class VLevel3 : public VLevel1, public VLevel2 {
public:
    int vl3_value;
    VLevel3() : vl3_value(13000) {}
    virtual const char* vname() const override { return "VLevel3"; }
};

// ========== 测试函数 ==========

// 测试深层单继承层次遍历
int test_deep_single_inheritance() {
    printf("=== Test Deep Single Inheritance (5 levels) ===\n");

    Level5 obj;
    int failed = 0;

    // 测试向上转型到每一层
    Level1* l1 = &obj;
    Level2* l2 = &obj;
    Level3* l3 = &obj;
    Level4* l4 = &obj;
    Level5* l5 = &obj;

    if (l1 && l2 && l3 && l4 && l5) {
        printf("[PASS] All levels accessible\n");
        printf("  Level1 offset: %ld\n", (char*)l1 - (char*)&obj);
        printf("  Level2 offset: %ld\n", (char*)l2 - (char*)&obj);
        printf("  Level3 offset: %ld\n", (char*)l3 - (char*)&obj);
        printf("  Level4 offset: %ld\n", (char*)l4 - (char*)&obj);
        printf("  Level5 offset: %ld\n", (char*)l5 - (char*)&obj);
    } else {
        printf("[FAIL] Some levels not accessible\n");
        failed = 1;
    }

    // 测试 dynamic_cast 向下转型
    Level1* base = &obj;
    Level5* derived = dynamic_cast<Level5*>(base);
    if (derived != nullptr) {
        printf("[PASS] Level1* -> Level5* (dynamic_cast through 4 levels)\n");
    } else {
        printf("[FAIL] Level1* -> Level5* (dynamic_cast failed)\n");
        failed = 1;
    }

    // 测试类型信息
    const std::type_info& ti = typeid(obj);
    printf("  Type name: %s\n", ti.name());

    return failed;
}

// 测试复杂多重继承层次遍历
int test_complex_multiple_inheritance() {
    printf("\n=== Test Complex Multiple Inheritance ===\n");

    DerivedAB obj;
    int failed = 0;

    // 测试向上转型到所有基类
    BaseA* base_a = &obj;
    BaseB* base_b = &obj;
    MiddleAB* middle = &obj;

    if (base_a && base_b && middle) {
        printf("[PASS] All bases accessible\n");
        printf("  BaseA offset: %ld\n", (char*)base_a - (char*)&obj);
        printf("  BaseB offset: %ld\n", (char*)base_b - (char*)&obj);
        printf("  MiddleAB offset: %ld\n", (char*)middle - (char*)&obj);
    } else {
        printf("[FAIL] Some bases not accessible\n");
        failed = 1;
    }

    // 测试交叉转换
    BaseA* a_ptr = &obj;
    BaseB* b_ptr = dynamic_cast<BaseB*>(a_ptr);
    if (b_ptr != nullptr) {
        printf("[PASS] BaseA* -> BaseB* (cross-cast through MiddleAB)\n");
    } else {
        printf("[FAIL] BaseA* -> BaseB* (cross-cast failed)\n");
        failed = 1;
    }

    // 测试向下转型
    BaseA* base = &obj;
    DerivedAB* derived = dynamic_cast<DerivedAB*>(base);
    if (derived != nullptr) {
        printf("[PASS] BaseA* -> DerivedAB* (down-cast through MiddleAB)\n");
    } else {
        printf("[FAIL] BaseA* -> DerivedAB* (down-cast failed)\n");
        failed = 1;
    }

    return failed;
}

// 测试虚拟继承深层层次遍历
int test_deep_virtual_inheritance() {
    printf("\n=== Test Deep Virtual Inheritance ===\n");

    VLevel3 obj;
    int failed = 0;

    // 测试虚拟基类访问
    VirtualBase* vbase = &obj;
    VLevel1* vl1 = &obj;
    VLevel2* vl2 = &obj;

    if (vbase && vl1 && vl2) {
        printf("[PASS] All virtual bases accessible\n");
        printf("  VirtualBase offset: %ld\n", (char*)vbase - (char*)&obj);
        printf("  VLevel1 offset: %ld\n", (char*)vl1 - (char*)&obj);
        printf("  VLevel2 offset: %ld\n", (char*)vl2 - (char*)&obj);
    } else {
        printf("[FAIL] Some virtual bases not accessible\n");
        failed = 1;
    }

    // 测试虚拟基类唯一性
    VirtualBase* vb_from_vl1 = static_cast<VirtualBase*>(vl1);
    VirtualBase* vb_from_vl2 = static_cast<VirtualBase*>(vl2);

    if (vb_from_vl1 == vb_from_vl2) {
        printf("[PASS] Single VirtualBase instance (diamond resolved)\n");
    } else {
        printf("[FAIL] Multiple VirtualBase instances\n");
        failed = 1;
    }

    // 测试向下转型
    VirtualBase* base = &obj;
    VLevel3* derived = dynamic_cast<VLevel3*>(base);
    if (derived != nullptr) {
        printf("[PASS] VirtualBase* -> VLevel3* (dynamic_cast through virtual inheritance)\n");
    } else {
        printf("[FAIL] VirtualBase* -> VLevel3* (dynamic_cast failed)\n");
        failed = 1;
    }

    return failed;
}

// 测试异常处理中的深层类型匹配
int test_exception_deep_matching() {
    printf("\n=== Test Exception Deep Type Matching ===\n");

    int failed = 0;

    // 测试1：抛出 Level5，捕获 Level1（跨越4层）
    try {
        throw Level5();
    } catch (const Level1& e) {
        printf("[PASS] Level5 exception caught by Level1 handler (4 levels deep)\n");
        printf("  Exception type: %s\n", typeid(e).name());
    } catch (...) {
        printf("[FAIL] Level5 exception not caught by Level1 handler\n");
        failed = 1;
    }

    // 测试2：抛出 Level5，捕获 Level3（跨越2层）
    try {
        throw Level5();
    } catch (const Level3& e) {
        printf("[PASS] Level5 exception caught by Level3 handler (2 levels deep)\n");
    } catch (...) {
        printf("[FAIL] Level5 exception not caught by Level3 handler\n");
        failed = 1;
    }

    // 测试3：抛出 DerivedAB，捕获 BaseA
    try {
        throw DerivedAB();
    } catch (const BaseA& e) {
        printf("[PASS] DerivedAB exception caught by BaseA handler\n");
    } catch (...) {
        printf("[FAIL] DerivedAB exception not caught by BaseA handler\n");
        failed = 1;
    }

    // 测试4：抛出 DerivedAB，捕获 BaseB（跨越 MiddleAB）
    try {
        throw DerivedAB();
    } catch (const BaseB& e) {
        printf("[PASS] DerivedAB exception caught by BaseB handler (through MiddleAB)\n");
    } catch (...) {
        printf("[FAIL] DerivedAB exception not caught by BaseB handler\n");
        failed = 1;
    }

    // 测试5：抛出 VLevel3，捕获 VirtualBase（虚拟继承）
    try {
        throw VLevel3();
    } catch (const VirtualBase& e) {
        printf("[PASS] VLevel3 exception caught by VirtualBase handler (virtual inheritance)\n");
    } catch (...) {
        printf("[FAIL] VLevel3 exception not caught by VirtualBase handler\n");
        failed = 1;
    }

    return failed;
}

// 测试动态类型信息解析
int test_dynamic_type_info() {
    printf("\n=== Test Dynamic Type Info Resolution ===\n");

    int failed = 0;

    // 测试1：使用 std::current_exception
    try {
        throw Level5();
    } catch (...) {
        std::exception_ptr eptr = std::current_exception();
        if (eptr) {
            try {
                std::rethrow_exception(eptr);
            } catch (const Level5& e) {
                printf("[PASS] std::current_exception works with Level5\n");
                printf("  Type: %s\n", typeid(e).name());
            } catch (...) {
                printf("[FAIL] std::current_exception type mismatch\n");
                failed = 1;
            }
        } else {
            printf("[FAIL] std::current_exception returned null\n");
            failed = 1;
        }
    }

    // 测试2：嵌套异常
    try {
        try {
            throw Level3();
        } catch (...) {
            std::throw_with_nested(Level5());
        }
    } catch (const Level5& e) {
        printf("[PASS] Nested exception (outer) caught correctly\n");
        try {
            std::rethrow_if_nested(e);
        } catch (const Level3& nested) {
            printf("[PASS] Nested exception (inner) caught correctly\n");
            printf("  Inner type: %s\n", typeid(nested).name());
        } catch (...) {
            printf("[FAIL] Nested exception (inner) type mismatch\n");
            failed = 1;
        }
    } catch (...) {
        printf("[FAIL] Nested exception (outer) not caught\n");
        failed = 1;
    }

    return failed;
}

// 测试类型遍历性能
int test_type_traversal_performance() {
    printf("\n=== Test Type Traversal Performance ===\n");

    int failed = 0;
    const int iterations = 10000;

    // 测试深层继承的 dynamic_cast 性能
    Level5 obj;
    Level1* base = &obj;

    // 简化性能测试（不使用 chrono）
    for (int i = 0; i < iterations; i++) {
        Level5* derived = dynamic_cast<Level5*>(base);
        if (!derived) {
            printf("[FAIL] dynamic_cast failed in performance test\n");
            failed = 1;
            break;
        }
    }

    if (failed == 0) {
        printf("[PASS] %d dynamic_cast operations completed successfully\n", iterations);
    }

    return failed;
}

int main() {
    printf("========================================\n");
    printf("  Type Hierarchy & Dynamic Type Test\n");
    printf("========================================\n\n");

    int failed = 0;

    failed += test_deep_single_inheritance();
    failed += test_complex_multiple_inheritance();
    failed += test_deep_virtual_inheritance();
    failed += test_exception_deep_matching();
    failed += test_dynamic_type_info();
    failed += test_type_traversal_performance();

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
