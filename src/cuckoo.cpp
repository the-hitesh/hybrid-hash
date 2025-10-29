#include "cuckoo.h"
}


template<typename K, typename V>
bool CuckooComponent<K,V>::insert(const K& key, const V& value, std::vector<Bucket>& table) {
size_t cap = table.size();
size_t idx1 = h1(key) & (cap - 1);
if (!table[idx1].occupied) { table[idx1] = {key, value, true}; return true; }
size_t idx2 = h2(key) & (cap - 1);
if (!table[idx2].occupied) { table[idx2] = {key, value, true}; return true; }


size_t cur_idx = idx1;
K cur_k = key;
V cur_v = value;
for (size_t kick = 0; kick < max_kicks_; ++kick) {
std::swap(cur_k, table[cur_idx].key);
std::swap(cur_v, table[cur_idx].value);
table[cur_idx].occupied = true;
// compute alternate
size_t alt1 = h1(cur_k) & (cap - 1);
size_t alt2 = h2(cur_k) & (cap - 1);
cur_idx = (cur_idx == alt1) ? alt2 : alt1;
if (!table[cur_idx].occupied) { table[cur_idx] = {cur_k, cur_v, true}; return true; }
}
return false;
}


template<typename K, typename V>
std::optional<V> CuckooComponent<K,V>::lookup(const K& key, const std::vector<Bucket>& table) const {
size_t cap = table.size();
size_t idx1 = h1(key) & (cap - 1);
if (table[idx1].occupied && table[idx1].key == key) return table[idx1].value;
size_t idx2 = h2(key) & (cap - 1);
if (table[idx2].occupied && table[idx2].key == key) return table[idx2].value;
return std::nullopt;
}


template<typename K, typename V>
bool CuckooComponent<K,V>::remove(const K& key, std::vector<Bucket>& table) {
size_t cap = table.size();
size_t idx1 = h1(key) & (cap - 1);
if (table[idx1].occupied && table[idx1].key == key) { table[idx1].occupied = false; return true; }
size_t idx2 = h2(key) & (cap - 1);
if (table[idx2].occupied && table[idx2].key == key) { table[idx2].occupied = false; return true; }
return false;
}


template<typename K, typename V>
void CuckooComponent<K,V>::rehash(std::vector<Bucket>& table, size_t new_capacity) {
std::vector<Bucket> newtab(new_capacity);
auto old = table; table.swap(newtab);
capacity_ = new_capacity;
for (auto &b : old) {
if (b.occupied) insert(b.key, b.value, table);
}
}


// Explicit template instantiation to avoid linker issues
template class CuckooComponent<int,int>;
template class CuckooComponent<size_t,size_t>;
