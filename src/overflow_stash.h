#pragma once
#include <vector>
#include <optional>
#include <mutex>


template<typename K, typename V>
class OverflowStash {
public:
OverflowStash(size_t capacity = 256);
bool insert(const K& key, const V& value);
std::optional<V> lookup(const K& key);
bool remove(const K& key);
void clear();


private:
struct Slot { K key; V value; bool occupied=false; };
std::vector<Slot> stash_;
std::mutex mtx_;
size_t capacity_;
};
