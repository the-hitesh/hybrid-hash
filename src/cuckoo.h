#pragma once
#include <vector>
#include <optional>
#include <functional>
#include <random>


template<typename K, typename V>
class CuckooComponent {
public:
struct Bucket { K key; V value; bool occupied=false; };
CuckooComponent(size_t capacity = 1024, size_t max_kicks = 500);


bool insert(const K& key, const V& value, std::vector<Bucket>& table);
std::optional<V> lookup(const K& key, const std::vector<Bucket>& table) const;
bool remove(const K& key, std::vector<Bucket>& table);
void rehash(std::vector<Bucket>& table, size_t new_capacity);


private:
size_t capacity_;
size_t max_kicks_;
std::hash<K> hasher_;
size_t h1(const K& key) const;
size_t h2(const K& key) const;
};
