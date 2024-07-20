#ifndef DINOCACHE_LRU_H
#define DINOCACHE_LRU_H

#include <chrono>
#include <mutex>
#include <list>
#include <map>
#include <cassert>
#include <unordered_map>

namespace dino
{
    namespace cache
    {
        using _clock = std::chrono::steady_clock;
        using duration = _clock::duration;
        using time_point = _clock::time_point;

        const duration TTL = std::chrono::seconds(20);

        template <typename K, typename V>
        class LRUCache
        {
        public:
            typedef K key_t;
            typedef V value_t;

            typedef std::multimap<time_point, key_t>
                timestamp_to_key_type;
            typedef std::unordered_map<
                key_t,
                std::pair<value_t, typename timestamp_to_key_type::iterator>>
                key_to_value_type;

            LRUCache(size_t _capacity = 3) : capacity(_capacity)
            {
                assert(_capacity >= 0);
            };

            ~LRUCache()
            {
                keyMap.clear();
                timeBuckets.clear();
            };

            // returns 1 on found/stored, 0 on not found/stored
            int get(const key_t &key, value_t &value);
            int put(const key_t &key, const value_t &value, const duration &ttl = TTL);

        private:
            inline bool isExpired(time_point expiryTime) { return _clock::now() > expiryTime; };

            void _evictExpired();
            void _evictLRU();
            void _update(const key_t &key, const value_t &value, const duration &ttl = TTL);

            /* TimeBuckets. <expiryTime, key that expire at expiryTime>  */
            timestamp_to_key_type timeBuckets;
            /* Holds iterator to cache and timeBuckets for each key
            Could have used pair<list<K>::iterator, list<LRUCacheEntry>::iterator> but
            created a struct for better readability
            */
            key_to_value_type keyMap;

            std::mutex _mutex;
            size_t capacity; // maximum capacity of cache
        };

    }
}

#include "LRUCache.hpp"

#endif
