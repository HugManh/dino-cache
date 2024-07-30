#ifndef DINOCACHE_LRU_HPP_
#define DINOCACHE_LRU_HPP_

#include <iostream>
#include <cstddef>
#include <vector>

namespace dino
{
    namespace cache
    {
        // updates the priority of data in the LRU cache.
        template <typename key_type, typename value_type>
        void
        LRUCache<key_type, value_type>::evictExpired()
        {
            std::cout << "start" << " " << __FUNCTION__ << std::endl;
            while (!timeBuckets_.empty() && isExpired(timeBuckets_.begin()->first))
            {
                auto it = timeBuckets_.begin();
                keyMap_.erase(it->second);
                timeBuckets_.erase(it);
                std::cout << __FUNCTION__ << " size " << keyMap_.size() << std::endl;
            }
            std::cout << "end" << " " << __FUNCTION__ << std::endl;
            return;
        }

        /// @brief Purge the least-recently-used element in the cache
        /// @tparam key_type
        /// @tparam value_type
        template <typename key_type, typename value_type>
        void
        LRUCache<key_type, value_type>::evict()
        {
            assert(!timeBuckets_.empty());

            auto oldest = timeBuckets_.begin();
            keyMap_.erase(oldest->second);
            timeBuckets_.erase(oldest);
            return;
        }

        // updates the priority of data in the LRU cache.
        template <typename key_type, typename value_type>
        void
        LRUCache<key_type, value_type>::update(const key_type &key, const value_type &value, const duration &ttl)
        {
            auto old = keyMap_.find(key);
            typename timestamp_to_key_type::iterator it_timestamp = old->second.second;

            // Remove the old node in keyMap_ & key in timeBuckets_
            keyMap_.erase(key);
            timeBuckets_.erase(it_timestamp);

            std::cout << "update: size = " << keyMap_.size() << std::endl;

            const time_point now = cclock::now();
            typename timestamp_to_key_type::iterator newit = timeBuckets_.insert(std::make_pair(now + ttl, key));
            keyMap_.emplace(key, std::make_pair(value, newit));

            return;
        }

        template <typename key_type, typename value_type>
        std::vector<key_type>
        LRUCache<key_type, value_type>::keys()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::vector<key_type> keys;
            for (const auto &it : keyMap_)
            {
                keys.push_back(it.first);
            }

            return keys;
        }

        template <typename key_type, typename value_type>
        int
        LRUCache<key_type, value_type>::get(const key_type &key, value_type &value)
        {
            std::lock_guard<std::mutex> lock(mutex_);

            evictExpired();

            auto it = keyMap_.find(key);
            if (it == keyMap_.end())
                return 0;

            typename timestamp_to_key_type::iterator it_timestamp = it->second.second;
            /* Expiry time */
            if (isExpired(it_timestamp->first))
            {
                std::cout << "====> get: key = " << key << " size = " << keyMap_.size() << std::endl;
                keyMap_.erase(key);
                timeBuckets_.erase(it_timestamp);
                return 0;
            }

            value = it->second.first;

            // update(key, value);

            return 1;
        }

        template <typename key_type, typename value_type>
        int
        LRUCache<key_type, value_type>::put(const key_type &key, const value_type &value, const duration &ttl)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::cout << "Put Key: " << key << " ttl " << ttl.count() << std::endl;
            auto el = keyMap_.find(key);
            if (el != keyMap_.end())
            {
                update(key, value, ttl);
                std::cout << "Element already exists: " << el->first << " => " << el->second.first << std::endl;
                return 1;
            }

            if (keyMap_.size() >= capacity_)
                evict(); /* Capacity reached */
            const time_point now = cclock::now();
            typename timestamp_to_key_type::iterator it = timeBuckets_.insert(std::make_pair(now + ttl, key));
            keyMap_.emplace(key, std::make_pair(value, it));
            // if (newel.second) {
            //   std::cout << "Add new element: " << newel.first->first << " => " << newel.first->second.first << std::endl;
            // }

            return 1;
        }
    } // namespace cache
} // namespace dino

#endif // DINOCACHE_LRU_HPP_
