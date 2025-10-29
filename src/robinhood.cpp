#include "robinhood.h"
#include <algorithm>


template<typename K, typename V>
RobinHoodComponent<K,V>::RobinHoodComponent(size_t capacity): capacity_(capacity) {}


template<typename K, typename V>
bool RobinHoodComponent<K,V>::insert(const K& key, const V& value, std::vector<Bucket>& table) {
size_t cap = table.size();
size_t idx = hasher_(key) & (cap - 1);
uint32_t probe = 0;
K curk = key;
V curv = value;
uint32_t curprobe = probe;
while (true) {
if (!table[idx].occupied) {
table[idx] = {curk, curv, true, curprobe};
return true;
}
if (table[idx].key == curk) {
table[idx].value = curv; // update
return true;
}
if (table[idx].probe_count < curprobe) {
// swap
std::swap(curk, table[idx].key);
std::swap(curv, table[idx].value);
std::swap(curprobe, table[idx].probe_count);
}
idx = (idx + 1) & (cap - 1);
++curprobe;
if (curprobe > cap) return false;
}
}


template<typename K, typename V>
std::optional<V> RobinHoodComponent<K,V>::lookup(const K& key, const std::vector<Bucket>& table) const {
size_t cap = table.size();
size_t idx = hasher_(key) & (cap - 1);
uint32_t probe = 0;
while (true) {
if (!table[idx].occupied) return std::nullopt;
if (table[idx].key == key) return table[idx].value;
if (table[idx].probe_count < probe) return std::nullopt; // can't be further
idx = (idx + 1) & (cap - 1);
++probe;
if (probe > cap) return std::nullopt;
}
}


template<typename K, typename V>
bool RobinHoodComponent<K,V>::remove(const K& key, std::vector<Bucket>& table) {
size_t cap = table.size();
size_t idx = hasher_(key) & (cap - 1);
uint32_t probe = 0;
template class RobinHoodComponent<int,int>;
