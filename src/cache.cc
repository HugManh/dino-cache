#include "cache.h"

namespace dino
{
    namespace cache
    {

        Cache::Cache(size_t capacity, Algorithm alg) : capacity_(capacity), type_cache_(alg)
        {
            Build();
        };

        void Cache::Build()
        {
            if (capacity_ <= 0 && type_cache_ != Algorithm::TYPE_SIMPLE)
                throw "Cache size <= 0";

            build();
            return;
        }

        std::vector<StringView> Cache::keys()
        {
            return lru_cache_->keys();
        }

        OptionalString Cache::get(const StringView &key)
        {
            StringView value;
            lru_cache_->get(key, value);
            if (value.empty())
                return std::nullopt;
            return value.data();
        }

        int Cache::put(const StringView &key, const StringView &value, const duration &ttl)
        {
            return lru_cache_->put(key, value, ttl);
        }

        void Cache::build()
        {
            switch (type_cache_)
            {
            case Algorithm::TYPE_LRU:
                lru_cache_ = new LRUCache<StringView, StringView>(capacity_);
                return;

            default:
                throw std::runtime_error("Unknown type cache algorithm");
            }
        }

    } // namespace cache

} // namespace dino
