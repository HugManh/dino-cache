#include <iostream>
#include <cstddef>
#include "cache.h"

namespace dino
{
    namespace cache
    {

        Cache::Cache(size_t capacity, Algorithm alg) : capacity_(capacity), type_cache_(alg)
        {
            Build();
        };

        void
        Cache::Build()
        {
            if (capacity_ <= 0 && type_cache_ != Algorithm::TYPE_SIMPLE)
                throw "Cache size <= 0";

            build();
            return;
        }

        std::vector<std::string>
        Cache::keys()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return lru_cache_->keys();
        }

        OptionalString
        Cache::get(const std::string &key)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::string value;
            lru_cache_->get(key, value);
            std::cout << "get: size " << lru_cache_->size() << std::endl;
            if (value.empty())
            {
                std::cout << "get: " << key.data() << " data empty" << std::endl;
                return std::nullopt;
            }

            std::cout << "get: " << key.data() << " => " << value.data() << std::endl;
            return value.data();
        }

        int
        Cache::put(const std::string &key, const std::string &value, int ttl)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return lru_cache_->put(key, value, std::chrono::seconds(ttl));
        }

        void
        Cache::build()
        {
            switch (type_cache_)
            {
            case Algorithm::TYPE_LRU:
                lru_cache_ = new LRUCache<std::string, std::string>(capacity_);
                return;

            default:
                throw std::runtime_error("Unknown type cache algorithm");
            }
        }

    } // namespace cache

} // namespace dino
