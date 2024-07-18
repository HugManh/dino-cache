#ifndef DINOCACHE_LRU_H
#define DINOCACHE_LRU_H

#include <chrono>
#include <mutex>
#include <list>
#include <map>
#include <unordered_map>

using _clock = std::chrono::steady_clock;
using duration = _clock::duration;
using time_point = _clock::time_point;

const duration TTL = std::chrono::seconds(20);

template <typename Key, typename Value>
class LRU
{
public:
    LRU(size_t _capacity = 3) : capacity(_capacity){};
    ~LRU(){};

    // returns 1 on found/stored, 0 on not found/stored
    int get(const Key &key, Value &value);
    int put(const Key &key, const Value &value, const duration &ttl = TTL);

private:
    /*LRUEntry that holds the key, value and the expiry time of a key, value pair*/
    struct LRUEntry
    {
        Key key;
        Value value;
        time_point expiryTime;

        LRUEntry() = default;

        LRUEntry(Key _key, Value _value, duration ttl = TTL) : key(_key), value(_value), expiryTime(_clock::now() + ttl){};
    };

    inline bool isExpired(time_point expiryTime) { return _clock::now() > expiryTime; };

    void _evictExpired();
    void _updateNode(const Key &key, const Value &value, const duration &ttl = TTL);
    void _evictLRU();

    /* TimeBuckets. <expiryTime, list<keys that expire at expiryTime>  */
    std::multimap<time_point, Key> timeBuckets;
    /* Holds iterator to cache and timeBuckets for each key
    Could have used pair<list<LRUEntry>::iterator, list<Key>::iterator> but
    created a struct for better readability
    */
    std::unordered_map<Key, LRUEntry> keyUMap;

    std::mutex _mutex;
    size_t capacity; // maximum capacity of cache
};

#include "LRU.hpp"

#endif
