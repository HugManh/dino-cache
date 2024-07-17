#ifndef DINOCACHE_LRU_HPP
#define DINOCACHE_LRU_HPP

#include <iostream>

// updates the priority of data in the LRU cache.
template <typename Key, typename Value>
void LRU<Key, Value>::_evictExpired()
{
    std::cout << "start" << " " << __FUNCTION__ << std::endl;
    std::multimap<time_point, Key> timeBuckets;
    while (!timeBuckets.empty() && isExpired(timeBuckets.begin()->first))
    {
        std::cout << "while" << __FUNCTION__ << std::endl;
        std::cout << "while" << timeBuckets.empty() << isExpired(timeBuckets.begin()->first) << __FUNCTION__ << std::endl;
        auto it = timeBuckets.begin();
        keyUMap.erase(it->second);
        timeBuckets.erase(it);
    }
    std::cout << "end" << " " << __FUNCTION__ << std::endl;
    return;
}

// updates the priority of data in the LRU cache.
template <typename Key, typename Value>
void LRU<Key, Value>::_updateNode(const Key &key, const Value &value, const duration &ttl)
{
    Node2 current_node = keyUMap[key];
    auto current_expiryTime = current_node.expiryTime;
    auto current_value = current_node.value;

    // Remove the old node in keyUMap & key in timeBuckets
    keyUMap.erase(key);
    auto range = timeBuckets.equal_range(current_expiryTime);

    for (auto it = range.first; it != range.second; ++it)
    {
        if (key == it->second)
        {
            timeBuckets.erase(it);
            break;
        }
    }

    // Push front the new node to keyUMap & timeBuckets
    Node2 node = Node2(key, value, ttl);
    keyUMap[key] = node;
    timeBuckets.emplace(node.expiryTime, key);

    return;
}

template <typename Key, typename Value>
void LRU<Key, Value>::_evictLRU()
{
    for (auto it = timeBuckets.begin(); it != timeBuckets.end(); ++it)
    {
        if (isExpired(it->first))
        {
            keyUMap.erase(it->second);
            timeBuckets.erase(it);
        }
        break;
    }

    return;
}

template <typename Key, typename Value>
std::optional<Value> LRU<Key, Value>::get(const Key &key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    // lazy evict on ttl.
    _evictExpired();

    /* Key in map */
    auto it = keyUMap.find(key);
    if (it == keyUMap.end())
        return std::nullopt;
    auto expiryTime = it->second.expiryTime;

    /* Expiry time */
    if (isExpired(expiryTime))
    {
        timeBuckets.erase(expiryTime);
        keyUMap.erase(it);
        return std::nullopt;
    }

    auto value = it->second.value;
    _updateNode(key, value);

    return value;
}

template <typename Key, typename Value>
void LRU<Key, Value>::set(const Key &key, const Value &value, const duration &ttl)
{
    std::cout << "set " << key << " " << value << std::endl;
    std::lock_guard<std::mutex> lock(_mutex);
    // lazy evict on ttl.
    _evictExpired();

    if (keyUMap.find(key) != keyUMap.end())
    {
        _updateNode(key, value, ttl);
        return;
    }

    if (keyUMap.size() >= capacity)
    {
        _evictLRU(); /* Capacity reached */
    }

    // Push front the new node to keyUMap & timeBucket
    Node2 node = Node2(key, value, ttl);
    keyUMap[key] = node;
    timeBuckets.emplace(node.expiryTime, key);

    return;
}

#endif
