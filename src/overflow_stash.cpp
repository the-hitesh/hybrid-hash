#include "overflow_stash.h"
#include <functional>


template<typename K, typename V>
OverflowStash<K,V>::OverflowStash(size_t capacity): capacity_(capacity) {
stash_.resize(capacity_);
}


template<typename K, typename V>
bool OverflowStash<K,V>::insert(const K& key, const V& value) {
std::lock_guard lock(mtx_);
size_t idx = std::hash<K>{}(key) % capacity_;
for (size_t i=0;i<capacity_;++i) {
size_t id=(idx+i)%capacity_;
if (!stash_[id].occupied) { stash_[id] = {key, value, true}; return true; }
if (stash_[id].occupied && stash_[id].key == key) { stash_[id].value = value; return true; }
}
return false;
}


template<typename K, typename V>
std::optional<V> OverflowStash<K,V>::lookup(const K& key) {
std::lock_guard lock(mtx_);
size_t idx = std::hash<K>{}(key) % capacity_;
for (size_t i=0;i<capacity_;++i) {
size_t id=(idx+i)%capacity_;
if (stash_[id].occupied && stash_[id].key == key) return stash_[id].value;
}
return std::nullopt;
}


template<typename K, typename V>
bool OverflowStash<K,V>::remove(const K& key) {
std::lock_guard lock(mtx_);
size_t idx = std::hash<K>{}(key) % capacity_;
for (size_t i=0;i<capacity_;++i) {
size_t id=(idx+i)%capacity_;
if (stash_[id].occupied && stash_[id].key == key) { stash_[id].occupied = false; return true; }
}
return false;
}


template<typename K, typename V>
void OverflowStash<K,V>::clear() {
std::lock_guard lock(mtx_);
for (auto &s : stash_) s.occupied = false;
}


template class OverflowStash<int,int>;
