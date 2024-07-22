#ifndef DINOCACHE_LRU_HPP_
#define DINOCACHE_LRU_HPP_

#include <iostream>

namespace dino
{
    namespace cache
    {
        // updates the priority of data in the LRU cache.
        template <typename key_type, typename value_type>
        void LRUCache<key_type, value_type>::_evictExpired()
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
        /// @tparam key_type
        /// @tparam value_type
        template <typename key_type, typename value_type>
        void LRUCache<key_type, value_type>::_evict()
        {
            assert(!timeBuckets.empty());

            auto oldest = timeBuckets.begin();
            keyMap.erase(oldest->second);
            timeBuckets.erase(oldest);
            return;
        }

        // updates the priority of data in the LRU cache.
        template <typename key_type, typename value_type>
        void LRUCache<key_type, value_type>::_update(const key_type &key, const value_type &value, const duration &ttl)
        {
            auto oldit = keyMap.find(key);
            if (oldit == keyMap.end())
                return;
            key_type oldkey = oldit->first;
            typename timestamp_to_key_type::iterator it_timestamp = oldit->second.second;

            // Remove the old node in keyMap & key in timeBuckets
            keyMap.erase(oldkey);
            timeBuckets.erase(it_timestamp);

            typename timestamp_to_key_type::iterator newit = timeBuckets.insert(std::make_pair(_clock::now() + TTL, key));
            keyMap.insert(std::make_pair(key, std::make_pair(value, newit)));

            return;
        }

        template <typename key_type, typename value_type>
        int LRUCache<key_type, value_type>::get(const key_type &key, value_type &value)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            /* key_type in map */
            auto it = keyMap.find(key);
            if (it == keyMap.end())
                return 0;
            typename timestamp_to_key_type::iterator it_timestamp = it->second.second;

            /* Expiry time */
            if (isExpired(it_timestamp->first))
            {
                keyMap.erase(key);
                timeBuckets.erase(it_timestamp);
                return 0;
            }

            value = it->second.first;

            _update(key, value);

            return 1;
        }

        template <typename key_type, typename value_type>
        int LRUCache<key_type, value_type>::put(const key_type &key, const value_type &value, const duration &ttl)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            if (keyMap.find(key) != keyMap.end())
            {
                _update(key, value, ttl);
                std::cout << "key exists, put " << key << " " << value << std::endl;
                return 1;
            }

            if (keyMap.size() >= capacity)
                _evict(); /* Capacity reached */

            typename timestamp_to_key_type::iterator it = timeBuckets.insert(std::make_pair(_clock::now() + TTL, key));
            keyMap.insert(std::make_pair(key, std::make_pair(value, it)));

            std::cout << "put " << key << " " << value << std::endl;
            return 1;
        }
    }
}

#endif // DINOCACHE_LRU_HPP_
