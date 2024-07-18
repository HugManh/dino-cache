#ifndef DINOCACHE_LRU_HPP
#define DINOCACHE_LRU_HPP

#include <iostream>

// updates the priority of data in the LRU cache.
template <typename K, typename V>
void LRU<K, V>::_evictExpired()
{
    std::cout << "start" << " " << __FUNCTION__ << std::endl;
    while (!timeBuckets.empty() && isExpired(timeBuckets.begin()->first))
    {
        std::cout << "while" << __FUNCTION__ << std::endl;
        std::cout << "while" << timeBuckets.empty() << isExpired(timeBuckets.begin()->first) << __FUNCTION__ << std::endl;
        auto it = timeBuckets.begin();
        keyMap.erase(it->second);
        timeBuckets.erase(it);
    }
    std::cout << "end" << " " << __FUNCTION__ << std::endl;
    return;
}

/// @brief Purge the least-recently-used element in the cache
/// @tparam K
/// @tparam V
template <typename K, typename V>
void LRU<K, V>::_evictLRU()
{
    assert(!timeBuckets.empty());

    auto oldest = timeBuckets.begin();
    keyMap.erase(oldest->second);
    timeBuckets.erase(oldest);
    return;
}

// updates the priority of data in the LRU cache.
template <typename K, typename V>
void LRU<K, V>::_update(const key_t &key, const value_t &value, const duration &ttl)
{
    auto it = keyMap.find(key);
    auto key = it->first;
    timestamp_to_key_type::iterator it_timestamp = it->second;

    // Remove the old node in keyMap & key in timeBuckets
    keyMap.erase(key);


    // Push front the new node to keyMap & timeBuckets
    LRUEntry node = LRUEntry(key, value, ttl);
    keyMap[key] = node;
    timeBuckets.emplace(node.expiryTime, key);

    return;
}

template <typename K, typename V>
int LRU<K, V>::get(const K &key, V &value)
{
    std::lock_guard<std::mutex> lock(_mutex);


    /* K in map */
    auto it = keyMap.find(key);
    if (it == keyMap.end())
        return 0;
    auto expiryTime = it->second.expiryTime;

    /* Expiry time */
    if (isExpired(expiryTime))
    {
        timeBuckets.erase(expiryTime);
        keyMap.erase(it);
        return 0;
    }

    value = it->second.value;
    // _update(key, value);

    return 1;
}

template <typename K, typename V>
int LRU<K, V>::put(const key_t &key, const value_t &value, const duration &ttl)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (keyMap.find(key) != keyMap.end())
    {
        _update(key, value, ttl);
        std::cout << "key exists, put " << key << " " << value << std::endl;
        return 1;
    }

    if (keyMap.size() >= capacity)
        _evictLRU(); /* Capacity reached */

    typename timestamp_to_key_type::iterator it = timeBuckets.insert(_clock::now() + TTL, key);
    keyMap.insert(key, std::make_pair(value, it));

    std::cout << "put " << key << " " << value << std::endl;
    return 1;
}

#endif
