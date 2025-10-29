#pragma once
#include <vector>
#include <optional>
#include <atomic>
#include <shared_mutex>
#include "cuckoo.h"
#include "hopscotch.h"
#include "robinhood.h"
#include "overflow_stash.h"


// A thin orchestrator that composes the three components over a shared capacity.
// For simplicity each component operates on its own view of the backing storage.


template<typename K, typename V>
class HybridHash {
public:
using Cuckoo = CuckooComponent<K,V>;
using Hop = HopscotchComponent<K,V>;
using Robin = RobinHoodComponent<K,V>;
using Stash = OverflowStash<K,V>;


HybridHash(size_t initial_capacity = 1024);


bool insert(const K& key, const V& value);
std::optional<V> lookup(const K& key);
bool remove(const K& key);
size_t size() const { return size_.load(); }
double load_factor() const { return double(size())/double(capacity_); }


private:
size_t capacity_;
std::atomic<size_t> size_{0};
mutable std::shared_mutex mu_;


// underlying per-component tables
std::vector<typename Cuckoo::Bucket> cuckoo_table_;
std::vector<typename Hop::Bucket> hop_table_;
std::vector<typename Robin::Bucket> robin_table_;


Cuckoo cuckoo_;
Hop hopscotch_;
Robin robinhood_;
Stash stash_;


void rehash(size_t new_capacity);
};


#include "HybridHash.cpp" // template implementation
