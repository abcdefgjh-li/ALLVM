#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#define VMP_PROTECT __attribute__((annotate("vmp")))
#define NOINLINE __attribute__((noinline))

class TestBaseError : public std::runtime_error {
public:
    explicit TestBaseError(const std::string &message)
        : std::runtime_error(message) {}

    virtual int tag() const noexcept { return 1; }
};

class TestDerivedError : public TestBaseError {
public:
    explicit TestDerivedError(const std::string &message)
        : TestBaseError(message) {}

    int tag() const noexcept override { return 2; }
};

struct ScopeMarker {
    int *state;

    explicit ScopeMarker(int *value) : state(value) {}

    ~ScopeMarker() {
        if (state != nullptr) {
            ++(*state);
        }
    }
};

struct TestCase {
    const char *name;
    std::function<bool(void)> run;
};

static bool expect_eq(const char *name, int actual, int expected) {
    if (actual == expected) {
        std::cout << "[PASS] " << name << " => " << actual << std::endl;
        return true;
    }

    std::cerr << "[FAIL] " << name << " => actual=" << actual
              << ", expected=" << expected << std::endl;
    return false;
}

static bool expect_true(const char *name, bool value, const std::string &detail) {
    if (value) {
        std::cout << "[PASS] " << name << " => " << detail << std::endl;
        return true;
    }

    std::cerr << "[FAIL] " << name << " => " << detail << std::endl;
    return false;
}

NOINLINE VMP_PROTECT int vmp_leaf_thrower(int code) {
    switch (code) {
    case 0:
        return 10;
    case 1:
        throw std::invalid_argument("invalid_argument from leaf");
    case 2:
        throw TestDerivedError("derived error from leaf");
    case 3:
        throw 77;
    case 4:
        throw std::logic_error("logic_error from leaf");
    default:
        throw std::runtime_error("unexpected test code");
    }
}

NOINLINE VMP_PROTECT int vmp_catch_and_translate(int code) {
    try {
        return vmp_leaf_thrower(code) + 1;
    } catch (const std::invalid_argument &) {
        throw TestDerivedError("translated invalid_argument");
    } catch (const TestBaseError &e) {
        return 200 + e.tag();
    } catch (...) {
        return 299;
    }
}

NOINLINE VMP_PROTECT int vmp_nested_rethrow(int code) {
    try {
        try {
            return vmp_catch_and_translate(code);
        } catch (const TestDerivedError &) {
            throw;
        }
    } catch (const TestBaseError &e) {
        return 300 + e.tag();
    }
}

NOINLINE VMP_PROTECT int vmp_exception_ptr_probe(int code) {
    std::exception_ptr ep;

    try {
        (void)vmp_leaf_thrower(code);
        return -1;
    } catch (...) {
        ep = std::current_exception();
    }

    try {
        std::rethrow_exception(ep);
    } catch (const std::invalid_argument &) {
        return 401;
    } catch (const TestDerivedError &) {
        return 402;
    } catch (int value) {
        return 400 + value;
    } catch (...) {
        return 499;
    }
}

NOINLINE VMP_PROTECT int vmp_raii_unwind(bool should_throw) {
    int destroyed = 0;

    try {
        ScopeMarker marker(&destroyed);
        if (should_throw) {
            throw std::logic_error("raii unwind");
        }
    } catch (const std::logic_error &) {
        return destroyed;
    }

    return destroyed;
}

NOINLINE VMP_PROTECT int vmp_top_level_flow(int code) {
    try {
        if (code == 4) {
            return vmp_leaf_thrower(code);
        }
        if ((code % 2) == 0) {
            return vmp_catch_and_translate(code);
        }
        return vmp_nested_rethrow(code);
    } catch (const std::exception &e) {
        std::ostringstream oss;
        oss << "top level observed: " << e.what();
        std::cout << oss.str() << std::endl;
        return -500;
    } catch (...) {
        return -599;
    }
}

int main() {
    std::vector<TestCase> tests = {
        {"no_throw_direct_path", []() { return expect_eq("no_throw_direct_path", vmp_leaf_thrower(0), 10); }},
        {"translate_std_exception", []() { return expect_eq("translate_std_exception", vmp_nested_rethrow(1), 302); }},
        {"catch_derived_as_base", []() { return expect_eq("catch_derived_as_base", vmp_catch_and_translate(2), 202); }},
        {"catch_non_std_with_ellipsis", []() { return expect_eq("catch_non_std_with_ellipsis", vmp_catch_and_translate(3), 299); }},
        {"exception_ptr_invalid_argument", []() { return expect_eq("exception_ptr_invalid_argument", vmp_exception_ptr_probe(1), 401); }},
        {"exception_ptr_derived", []() { return expect_eq("exception_ptr_derived", vmp_exception_ptr_probe(2), 402); }},
        {"exception_ptr_integer", []() { return expect_eq("exception_ptr_integer", vmp_exception_ptr_probe(3), 477); }},
        {"raii_unwind_throw", []() { return expect_eq("raii_unwind_throw", vmp_raii_unwind(true), 1); }},
        {"raii_unwind_no_throw", []() { return expect_eq("raii_unwind_no_throw", vmp_raii_unwind(false), 1); }},
        {"cross_vmp_call_no_throw", []() { return expect_eq("cross_vmp_call_no_throw", vmp_top_level_flow(0), 11); }},
        {"cross_vmp_call_exception", []() { return expect_eq("cross_vmp_call_exception", vmp_top_level_flow(1), 302); }},
        {"top_level_std_exception_visibility",
         []() {
             const int value = vmp_top_level_flow(4);
             return expect_true("top_level_std_exception_visibility", value == -500, "top-level catch for std::exception is reachable");
         }},
    };

    bool all_ok = true;
    std::cout << "[VMP-EXCEPTION-TEST] begin" << std::endl;

    for (const auto &test : tests) {
        try {
            if (!test.run()) {
                all_ok = false;
            }
        } catch (const std::exception &e) {
            all_ok = false;
            std::cerr << "[FAIL] " << test.name
                      << " => uncaught std::exception: " << e.what() << std::endl;
        } catch (...) {
            all_ok = false;
            std::cerr << "[FAIL] " << test.name
                      << " => uncaught unknown exception" << std::endl;
        }
    }

    std::cout << "[VMP-EXCEPTION-TEST] end => "
              << (all_ok ? "PASS" : "FAIL") << std::endl;
    return all_ok ? 0 : 1;
}
