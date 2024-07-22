#ifndef DINOCACHE_LRU_H_
#define DINOCACHE_LRU_H_

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

        template <typename key_type, typename value_type>
        class LRUCache
        {
        public:
            typedef std::multimap<time_point, key_type>
                timestamp_to_key_type;
            typedef std::unordered_map<
                key_type,
                std::pair<value_type, typename timestamp_to_key_type::iterator>>
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
            int get(const key_type &key, value_type &value);
            int put(const key_type &key, const value_type &value, const duration &ttl = TTL);

        private:
            inline bool isExpired(time_point expiryTime) { return _clock::now() > expiryTime; };

            void _evictExpired();
            void _evict();
            void _update(const key_type &key, const value_type &value, const duration &ttl = TTL);

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

#include "lru.hpp"

#endif // DINOCACHE_LRU_H_
