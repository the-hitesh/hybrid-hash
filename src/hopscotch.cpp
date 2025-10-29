#include "hopscotch.h"
#include <algorithm>
#include <cstring>


template<typename K, typename V>
HopscotchComponent<K,V>::HopscotchComponent(size_t capacity, size_t neighborhood)
: capacity_(capacity), neighborhood_(neighborhood) {}


template<typename K, typename V>
bool HopscotchComponent<K,V>::insert(const K& key, const V& value, std::vector<Bucket>& table) {
size_t cap = table.size();
size_t home = hasher_(key) & (cap - 1);
if (!table[home].occupied) { table[home] = {key, value, true, 1u<<0}; return true; }


// find free slot within some search limit (neighborhood * 4)
ssize_t free_idx = -1;
for (size_t i = 1; i < neighborhood_ * 4 && i < cap; ++i) {
size_t idx = (home + i) & (cap - 1);
if (!table[idx].occupied) { free_idx = idx; break; }
}
if (free_idx == -1) return false;
// move free slot closer using hopscotch algorithm
while (free_idx != (ssize_t)home) {
bool moved = false;
ssize_t start = std::max<ssize_t>((ssize_t)free_idx - (ssize_t)neighborhood_ + 1, 0);
for (ssize_t i = start; i < (ssize_t)free_idx; ++i) {
size_t idx = i & (cap - 1);
// check if idx's home can reach free_idx
size_t home_of_idx = hasher_(table[idx].key) & (cap - 1);
size_t dist = (free_idx - (ssize_t)idx + (ssize_t)cap) & (cap - 1);
if (table[idx].occupied && dist < (ssize_t)neighborhood_) {
// move
table[free_idx] = table[idx];
table[idx].occupied = false;
free_idx = idx;
moved = true;
break;
}
}
if (!moved) break;
}
if (free_idx - (ssize_t)home >= (ssize_t)neighborhood_) return false;
table[free_idx] = {key, value, true, 0};
// set hop bit on home
size_t offset = (free_idx - (ssize_t)home + (ssize_t)cap) & (cap - 1);
if (offset < 32) table[home].hop_info |= (1u << offset);
return true;
}


template<typename K, typename V>
std::optional<V> HopscotchComponent<K,V>::lookup(const K& key, const std::vector<Bucket>& table) const {
size_t cap = table.size();
size_t home = hasher_(key) & (cap - 1);
uint32_t hop = table[home].hop_info;
for (uint32_t b = 0; b < 32; ++b) {
if (hop & (1u << b)) {
size_t idx = (home + b) & (cap - 1);
if (table[idx].occupied && table[idx].key == key) return table[idx].value;
}
}
return std::nullopt;
}


template<typename K, typename V>
bool HopscotchComponent<K,V>::remove(const K& key, std::vector<Bucket>& table) {
size_t cap = table.size();
size_t home = hasher_(key) & (cap - 1);
uint32_t hop = table[home].hop_info;
for (uint32_t b = 0; b < 32; ++b) {
template class HopscotchComponent<int,int>;
