#include <iostream>
#include "HybridHash.h"


int main() {
HybridHash<int,int> h(1024);
for (int i=0;i<2000;i++) {
bool ok = h.insert(i, i*10);
if (!ok) std::cerr << "insert failed for " << i << "\n";
}
for (int i=0;i<2000;i++) {
auto v = h.lookup(i);
if (!v.has_value()) std::cerr << "missing " << i << "\n";
}
std::cout << "size: " << h.size() << " load: " << h.load_factor() << "\n";
return 0;
}
