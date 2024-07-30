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
            ~Cache()
            {
                if (lru_cache_)
                    free(lru_cache_);
            }

            void Build();

            std::vector<std::string> keys();
            OptionalString get(const std::string &key);
            int put(const std::string &key, const std::string &value, int ttl);

        private:
            void build();

            std::mutex mutex_;
            size_t capacity_;
            int type_cache_;

            LRUCache<std::string, std::string> *lru_cache_;
        };
    } // namespace cache
} // namespace dino

#endif // DINOCACHE_CACHE_H_
