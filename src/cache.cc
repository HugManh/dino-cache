#include "cache.h"

namespace dino
{
    namespace cache
    {
        void Cache::_init(Algorithm alg)
        {
            switch (alg)
            {
            case Algorithm::LRU:
                lru = new LRU<std::string, std::string>(capacity_);
                break;
            default:
                break;
            }
        }
    } // namespace cache

} // namespace dino
