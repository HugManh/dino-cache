#ifndef DINOCACHE_UTILS_H_
#define DINOCACHE_UTILS_H_

#include <optional>
#include <string>
#include <string_view>

namespace dino
{
    namespace cache
    {
        template <typename T>
        using Optional = std::optional<T>;
        
        using OptionalString = Optional<std::string>;
        using StringView = std::string_view;

    } // namespace cache

} // namespace dino

#endif // DINOCACHE_UTILS_H_
