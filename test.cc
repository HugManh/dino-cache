#include <iostream>
#include "LRU.h"

int main(int argc, char *argv[])
{
    LRU<std::string, std::string> *lru = new LRU<std::string, std::string>();
    std::cout << "Hello World!" << std::endl;
    return 0;
}
