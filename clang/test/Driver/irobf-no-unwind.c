// RUN: %clang -target aarch64-linux-android21 -### -c %s -firobf-no-unwind 2>&1 | FileCheck %s --check-prefix=DEFAULT
// RUN: %clang -target aarch64-linux-android21 -### -c %s -firobf-no-unwind -fasynchronous-unwind-tables -funwind-tables 2>&1 | FileCheck %s --check-prefix=FORCE

// DEFAULT: "-irobf-no-cfi"
// DEFAULT-NOT: "-funwind-tables=

// FORCE: "-irobf-no-cfi"
// FORCE-NOT: "-funwind-tables=

int no_unwind_driver_test(int x) { return x + 1; }
