#include <iostream>
#include <string>
#include <thread>
#include "cache.h"

using namespace dino::cache;

int main(int argc, char *argv[])
{
    Cache *cache = new Cache(30000);
    cache->put("test", "oke");
    cache->put("test1", "oke");
    cache->put("test2", "oke");
    cache->put("test3", "oke");
    cache->put("test4", "oke");
    cache->put("test5", "oke");
    OptionalString value = cache->get("test");
    std::cout << value.value() << std::endl;
    for (const auto &key : cache->keys())
    {
        std::cout << "--" << key << std::endl;
    }

    return 0;
}
