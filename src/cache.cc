#include "cache.h"

namespace dino
{
    namespace cache
    {
        OptionalString Cache::get(const StringView &key)
        {
            return std::nullopt;
        }

        int Cache::put(const StringView &key, const StringView &value)
        {
            return 1;
        }

        void Cache::build()
        {
            // switch (type_cache_)
            // {
            // case Algorithm::TYPE_LRU:
            //     lru_cache_ = new LRUCache<std::string, std::string>(capacity_);
            //     return;

            // default:
            //     throw std::runtime_error("Unknown type cache algorithm");
            // }
        }

    } // namespace cache

} // namespace dino
