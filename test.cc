#include <iostream>
#include <string>
#include <thread>
#include "cache.h"

using namespace dino::cache;

int main(int argc, char *argv[])
{
    Cache *cache = new Cache(30000);
    // OptionalString value = cache->get("test");
    // std::cout << value.value_or("Not found") << std::endl;
    return 0;
}
