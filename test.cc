#include <iostream>
#include <string>
#include <thread>
#include "LRU.h"

int main(int argc, char *argv[])
{
    LRU<std::string, std::string> *lru = new LRU<std::string, std::string>();
    lru->set("foo", "bar");
    lru->set("foo", "oke");
    lru->set("test", "???");
    lru->set("din", "oke");
    lru->set("din", "tret");
    lru->set("din", "trêtrt");
    std::cout << "---------------" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(18));
    std::cout << lru->get("foo").value_or("Not found key") << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(22));
    std::cout << "---------------" << std::endl;
    std::cout << lru->get("foo").value_or("Not found key") << std::endl;
    return 0;
}
