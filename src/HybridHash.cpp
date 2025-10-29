#include "HybridHash.h"
#include <algorithm>
#include <iostream>


template<typename K, typename V>
HybridHash<K,V>::HybridHash(size_t initial_capacity)
: capacity_(1ULL << (64 - __builtin_clzll(std::max<size_t>(1, initial_capacity-1))))
, cuckoo_(capacity_, 500)
, hopscotch_(capacity_, 32)
, robinhood_(capacity_)
, stash_(std::max<size_t>(64, capacity_/64))
{
cuckoo_table_.resize(capacity_);
hop_table_.resize(capacity_);
robin_table_.resize(capacity_);
}


template<typename K, typename V>
void HybridHash<K,V>::rehash(size_t new_capacity) {
std::unique_lock lock(mu_);
new_capacity = 1ULL << (64 - __builtin_clzll(std::max<size_t>(1, new_capacity-1)));
// rehash each component's table
cuckoo_.rehash(cuckoo_table_, new_capacity);
hopscotch_.rehash(hop_table_, new_capacity);
robinhood_.rehash(robin_table_, new_capacity);
// stash remains; we will try to drain it into new table later
capacity_ = new_capacity;
}


// Insert strategy: choose based on load factor
template<typename K, typename V>
bool HybridHash<K,V>::insert(const K& key, const V& value) {
std::unique_lock lock(mu_);
double lf = load_factor();
bool ok = false;
if (lf < 0.5) {
ok = robinhood_.insert(key, value, robin_table_);
if (ok) { size_.fetch_add(1); return true; }
}
if (lf < 0.8) {
ok = hopscotch_.insert(key, value, hop_table_);
if (ok) { size_.fetch_add(1); return true; }
}
// try cuckoo as fallback/high-load
// template class HybridHash<int,int>;
