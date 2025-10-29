#pragma once
#include <vector>
#include <optional>
#include <cstdint>


template<typename K, typename V>
class HopscotchComponent {
public:
struct Bucket { K key; V value; bool occupied = false; uint32_t hop_info = 0; };
HopscotchComponent(size_t capacity=1024, size_t neighborhood=32);


bool insert(const K& key, const V& value, std::vector<Bucket>& table);
std::optional<V> lookup(const K& key, const std::vector<Bucket>& table) const;
bool remove(const K& key, std::vector<Bucket>& table);
void rehash(std::vector<Bucket>& table, size_t new_capacity);


private:
size_t capacity_;
size_t neighborhood_;
std::hash<K> hasher_;
};
