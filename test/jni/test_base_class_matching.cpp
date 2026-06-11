// 测试基类匹配和类型层次遍历
#include <stdio.h>
#include <typeinfo>

// 基类
class Base {
public:
    int base_value;
    Base() : base_value(100) {}
    virtual ~Base() {}
    virtual const char* name() const { return "Base"; }
};

// 单继承派生类
class Derived : public Base {
public:
    int derived_value;
    Derived() : derived_value(200) {}
    virtual const char* name() const override { return "Derived"; }
};

// 另一个基类
class Base2 {
public:
    int base2_value;
    Base2() : base2_value(300) {}
    virtual ~Base2() {}
    virtual const char* name2() const { return "Base2"; }
};

// 多重继承派生类
class MultiDerived : public Base, public Base2 {
public:
    int multi_value;
    MultiDerived() : multi_value(400) {}
    virtual const char* name() const override { return "MultiDerived"; }
};

// 更深的继承层次
class GrandDerived : public Derived {
public:
    int grand_value;
    GrandDerived() : grand_value(500) {}
    virtual const char* name() const override { return "GrandDerived"; }
};

// 测试单继承的基类匹配
int test_single_inheritance() {
    printf("=== Test Single Inheritance ===\n");

    Derived derived;
    Base* base_ptr = &derived;

    // 测试 dynamic_cast 向上转型
    if (dynamic_cast<Base*>(base_ptr) != nullptr) {
        printf("[PASS] Derived* can be cast to Base*\n");
    } else {
        printf("[FAIL] Derived* cannot be cast to Base*\n");
        return 1;
    }

    // 测试 dynamic_cast 向下转型
    Base* base = new Derived();
    if (dynamic_cast<Derived*>(base) != nullptr) {
        printf("[PASS] Base* (pointing to Derived) can be cast to Derived*\n");
    } else {
        printf("[FAIL] Base* (pointing to Derived) cannot be cast to Derived*\n");
        delete base;
        return 1;
    }

    // 测试类型信息
    const std::type_info& base_type = typeid(Base);
    const std::type_info& derived_type = typeid(Derived);

    printf("Base type: %s\n", base_type.name());
    printf("Derived type: %s\n", derived_type.name());

    delete base;
    return 0;
}

// 测试多重继承的基类匹配
int test_multiple_inheritance() {
    printf("\n=== Test Multiple Inheritance ===\n");

    MultiDerived multi;
    Base* base_ptr = &multi;
    Base2* base2_ptr = &multi;

    // 测试向第一个基类的转换
    if (dynamic_cast<Base*>(base_ptr) != nullptr) {
        printf("[PASS] MultiDerived* can be cast to Base*\n");
    } else {
        printf("[FAIL] MultiDerived* cannot be cast to Base*\n");
        return 1;
    }

    // 测试向第二个基类的转换
    if (dynamic_cast<Base2*>(base2_ptr) != nullptr) {
        printf("[PASS] MultiDerived* can be cast to Base2*\n");
    } else {
        printf("[FAIL] MultiDerived* cannot be cast to Base2*\n");
        return 1;
    }

    // 测试交叉转换
    Base* base_from_multi = &multi;
    if (dynamic_cast<Base2*>(base_from_multi) != nullptr) {
        printf("[PASS] Base* (from MultiDerived) can be cross-cast to Base2*\n");
    } else {
        printf("[FAIL] Base* (from MultiDerived) cannot be cross-cast to Base2*\n");
        return 1;
    }

    // 测试类型信息
    const std::type_info& multi_type = typeid(MultiDerived);
    printf("MultiDerived type: %s\n", multi_type.name());

    return 0;
}

// 测试深层继承层次
int test_deep_hierarchy() {
    printf("\n=== Test Deep Hierarchy ===\n");

    GrandDerived grand;
    Base* base_ptr = &grand;

    // 测试向上转型到顶层基类
    if (dynamic_cast<Base*>(base_ptr) != nullptr) {
        printf("[PASS] GrandDerived* can be cast to Base*\n");
    } else {
        printf("[FAIL] GrandDerived* cannot be cast to Base*\n");
        return 1;
    }

    // 测试向上转型到中间基类
    if (dynamic_cast<Derived*>(base_ptr) != nullptr) {
        printf("[PASS] GrandDerived* can be cast to Derived*\n");
    } else {
        printf("[FAIL] GrandDerived* cannot be cast to Derived*\n");
        return 1;
    }

    // 测试类型信息
    const std::type_info& grand_type = typeid(GrandDerived);
    printf("GrandDerived type: %s\n", grand_type.name());

    return 0;
}

// 测试异常处理中的基类匹配
int test_exception_base_matching() {
    printf("\n=== Test Exception Base Matching ===\n");

    int failed = 0;

    // 测试1：捕获派生类异常，使用基类处理器
    try {
        throw Derived();
    } catch (const Base& e) {
        printf("[PASS] Derived exception caught by Base handler\n");
        printf("  Exception type: %s\n", typeid(e).name());
        printf("  Base value: %d\n", e.base_value);
    } catch (...) {
        printf("[FAIL] Derived exception not caught by Base handler\n");
        failed = 1;
    }

    // 测试2：捕获派生类异常，使用派生类处理器
    try {
        throw Derived();
    } catch (const Derived& e) {
        printf("[PASS] Derived exception caught by Derived handler\n");
        printf("  Exception type: %s\n", typeid(e).name());
        printf("  Base value: %d, Derived value: %d\n", e.base_value, e.derived_value);
    } catch (const Base& e) {
        printf("[FAIL] Derived exception caught by Base handler (should be caught by Derived handler first)\n");
        failed = 1;
    } catch (...) {
        printf("[FAIL] Derived exception not caught\n");
        failed = 1;
    }

    // 测试3：捕获深层继承异常
    try {
        throw GrandDerived();
    } catch (const GrandDerived& e) {
        printf("[PASS] GrandDerived exception caught by GrandDerived handler\n");
    } catch (const Derived& e) {
        printf("[FAIL] GrandDerived exception caught by Derived handler (should be caught by GrandDerived handler first)\n");
        failed = 1;
    } catch (const Base& e) {
        printf("[FAIL] GrandDerived exception caught by Base handler (should be caught by GrandDerived handler first)\n");
        failed = 1;
    } catch (...) {
        printf("[FAIL] GrandDerived exception not caught\n");
        failed = 1;
    }

    // 测试4：捕获深层继承异常，使用基类处理器
    try {
        throw GrandDerived();
    } catch (const Base& e) {
        printf("[PASS] GrandDerived exception caught by Base handler\n");
        printf("  Exception type: %s\n", typeid(e).name());
    } catch (...) {
        printf("[FAIL] GrandDerived exception not caught by Base handler\n");
        failed = 1;
    }

    // 测试5：捕获多重继承异常
    try {
        throw MultiDerived();
    } catch (const Base& e) {
        printf("[PASS] MultiDerived exception caught by Base handler\n");
        printf("  Exception type: %s\n", typeid(e).name());
    } catch (...) {
        printf("[FAIL] MultiDerived exception not caught by Base handler\n");
        failed = 1;
    }

    // 测试6：捕获多重继承异常，使用第二个基类处理器
    try {
        throw MultiDerived();
    } catch (const Base2& e) {
        printf("[PASS] MultiDerived exception caught by Base2 handler\n");
        printf("  Exception type: %s\n", typeid(e).name());
    } catch (...) {
        printf("[FAIL] MultiDerived exception not caught by Base2 handler\n");
        failed = 1;
    }

    return failed;
}

// 测试 dynamic_cast 失败情况
int test_dynamic_cast_failures() {
    printf("\n=== Test Dynamic Cast Failures ===\n");

    int failed = 0;

    // 测试向下转型失败
    Base base;
    Derived* derived_ptr = dynamic_cast<Derived*>(&base);

    if (derived_ptr == nullptr) {
        printf("[PASS] Base* cannot be cast to Derived* (correctly returns nullptr)\n");
    } else {
        printf("[FAIL] Base* incorrectly cast to Derived*\n");
        failed = 1;
    }

    // 测试交叉转换失败
    Base base_obj;
    Base2* base2_ptr = dynamic_cast<Base2*>(&base_obj);

    if (base2_ptr == nullptr) {
        printf("[PASS] Base* cannot be cross-cast to Base2* (correctly returns nullptr)\n");
    } else {
        printf("[FAIL] Base* incorrectly cross-cast to Base2*\n");
        failed = 1;
    }

    return failed;
}

int main() {
    printf("========================================\n");
    printf("  Base Class Matching Test Suite\n");
    printf("========================================\n\n");

    int failed = 0;

    failed += test_single_inheritance();
    failed += test_multiple_inheritance();
    failed += test_deep_hierarchy();
    failed += test_exception_base_matching();
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
