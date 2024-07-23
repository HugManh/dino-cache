#include <iostream>
#include <string>
#include <thread>
#include "cache.h"

using namespace dino::cache;

int main(int argc, char *argv[])
{
    Cache *cache = new Cache(30000);
    if (cache->put("test", "oke"))
    {
        std::cout << "hfdshf" << std::endl;
    }
    OptionalString value = cache->get("test");
    std::cout << value.value() << std::endl;
    return 0;
}
