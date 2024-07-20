#ifndef DINOCACHE_CACHE_H
#define DINOCACHE_CACHE_H

#include <string>
#include "LRU.h"
#include "algorithm.h"

namespace dino
{
    namespace cache
    {
        class Cache
        {
        public:
            /// @brief
            /// @param  alg the cache algorithm
            Cache(Algorithm alg)
            {
                _init(alg);
            };

        private:
            void _init(Algorithm alg = Algorithm::LRU);

            size_t capacity_;
            LRUCache<std::string, std::string> *lru;
        };
    } // namespace cache
} // namespace dino

#endif
