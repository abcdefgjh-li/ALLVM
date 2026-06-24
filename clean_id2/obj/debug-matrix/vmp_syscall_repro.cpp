#include <cstdlib>
__attribute__((annotate("vmp")))
int vm_entry() {
  exit(0);
  return 7;
}
int main() { return vm_entry(); }
