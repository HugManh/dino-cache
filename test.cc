#include <iostream>
#include <string>
#include <thread>
#include "LRU.h"

int main(int argc, char *argv[])
{
    LRU<std::string, std::string> *lru = new LRU<std::string, std::string>();
    lru->put("foo", "bar");
    lru->put("foo", "oke");
    std::string _value;
    if (lru->get("foo", _value))
    {
        std::cout << "-----" << _value << std::endl;
    }
    _value.clear();
    lru->put("test", "???");
    lru->put("din", "oke");
    lru->put("din", "tret");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    lru->put("din", "trêtrt");
    std::this_thread::sleep_for(std::chrono::seconds(8));
    std::cout << "---------------" << std::endl;
    if (lru->get("foo", _value))
    {
        std::cout << _value << std::endl;
    }
    _value.clear();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "---------------" << std::endl;
    if (lru->get("din", _value))
    {
        std::cout << _value << std::endl;
    }
    return 0;
}
