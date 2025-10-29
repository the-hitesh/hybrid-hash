#include "HybridHash.h"
if (ok) { size_.fetch_add(1); return true; }
}
// try cuckoo as fallback/high-load
ok = cuckoo_.insert(key, value, cuckoo_table_);
if (ok) { size_.fetch_add(1); return true; }


// try other methods if primary failed
ok = robinhood_.insert(key, value, robin_table_);
if (ok) { size_.fetch_add(1); return true; }
ok = hopscotch_.insert(key, value, hop_table_);
if (ok) { size_.fetch_add(1); return true; }


// stash fallback
ok = stash_.insert(key, value);
if (ok) { return true; }


// rehash and retry once
rehash(capacity_ * 2);
// After rehash, try again (recursive but will succeed or stash)
if (insert(key, value)) return true;
return false;
}


template<typename K, typename V>
std::optional<V> HybridHash<K,V>::lookup(const K& key) {
std::shared_lock lock(mu_);
auto v = robinhood_.lookup(key, robin_table_);
if (v.has_value()) return v;
v = hopscotch_.lookup(key, hop_table_);
if (v.has_value()) return v;
v = cuckoo_.lookup(key, cuckoo_table_);
if (v.has_value()) return v;
return stash_.lookup(key);
}


template<typename K, typename V>
bool HybridHash<K,V>::remove(const K& key) {
std::unique_lock lock(mu_);
bool ok = false;
ok = robinhood_.remove(key, robin_table_);
if (ok) { size_.fetch_sub(1); return true; }
ok = hopscotch_.remove(key, hop_table_);
if (ok) { size_.fetch_sub(1); return true; }
ok = cuckoo_.remove(key, cuckoo_table_);
if (ok) { size_.fetch_sub(1); return true; }
ok = stash_.remove(key);
if (ok) return true;
return false;
}


// Explicit template instantiation for common types if desired in translation unit
// template class HybridHash<int,int>;
