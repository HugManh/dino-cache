#ifndef DINOCACHE_LRU_H_
#define DINOCACHE_LRU_H_

#include <chrono>
#include <mutex>
#include <cassert>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

namespace dino
{
    namespace cache
    {
        using cclock = std::chrono::steady_clock;
        using duration = cclock::duration;
        using time_point = cclock::time_point;

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

            /// @brief Get list of keys in cache
            /// @return list of keys
            std::vector<key_type> keys();

            // /// @brief Get list of keys in cache
            // /// @return list of keys
            // std::unordered_map<key_type, value_type> getall();

            /// @brief Get the value at the specified key in the cache
            /// @param key The key to value
            /// @param value The value of key
            /// @return 1 on found, 0 on not found
            int get(const key_type &key, value_type &value);

            /// @brief Put the value at the specified key in the cache
            /// @param key The key to value
            /// @param value The value of key
            /// @param ttl The time to live in seconds (optional)
            /// @return 1 on stored, 0 on not stored
            int put(const key_type &key, const value_type &value, const duration &ttl = TTL);

        private:
            inline bool isExpired(time_point expiryTime) { return cclock::now() > expiryTime; };

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
