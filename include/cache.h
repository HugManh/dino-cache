#ifndef DINOCACHE_CACHE_H_
#define DINOCACHE_CACHE_H_

#include <string>
#include "lru.h"
#include "algorithm.h"
#include "utils.h"

namespace dino
{
    namespace cache
    {
        class Cache
        {
        public:
            Cache(size_t capacity, Algorithm alg = Algorithm::TYPE_LRU);

            void Build();

            int put(const StringView &key, const StringView &value, const duration &ttl = TTL);
            OptionalString get(const StringView &key);

        private:
            void build();

            size_t capacity_;
            int type_cache_;

            LRUCache<StringView, StringView> *lru_cache_;
        };
    } // namespace cache
} // namespace dino

#endif // DINOCACHE_CACHE_H_
