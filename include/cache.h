#ifndef DINOCACHE_CACHE_H_
#define DINOCACHE_CACHE_H_

#include <string>
#include "LRU.h"
#include "algorithm.h"
#include "utils.h"

namespace dino
{
    namespace cache
    {
        class Cache
        {
        public:
            Cache(size_t capacity, Algorithm alg = Algorithm::TYPE_LRU) : capacity_(capacity), type_cache_(alg)
            {
                if (capacity_ <= 0 && type_cache_ != Algorithm::TYPE_SIMPLE)
                    throw "Cache size <= 0";

                build();
            };

            OptionalString get(const StringView &key);
            int put(const StringView &key, const StringView &value);

        private:
            void build();

            size_t capacity_;
            int type_cache_;

            LRUCache<std::string, std::string> *lru_cache_;
        };
    } // namespace cache
} // namespace dino

#endif // DINOCACHE_CACHE_H_
