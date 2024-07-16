#include <chrono>
#include <mutex>
#include <list>
#include <unordered_map>

using clock = std::chrono::steady_clock;
using duration = clock::duration;
using time_point = clock::time_point;

template <typename Key, typename Value>
class LRU
{
public:
    LRU(size_t _capacity = 3) : capacity(_capacity){};
    ~LRU(){};
    bool get(const Key &key, const Value &value);
    void set(const Key &key, const Value &value);

private:
    const duration TTL = std::chrono::seconds(20);

    /*Node that holds the key, value and the expiry time of a key, value pair*/
    struct Node
    {
        Key key;
        Value value;
        time_point expiryTime;

        Node(Key _key, Value _value, time_point _expiryTime = clock::now()) : key(_key), value(_value), expiryTime(_expiryTime) {}
    };

    struct IteratorsContainer
    {
        std::list<Node>::iterator cacheIter;
        std::list<Key>::iterator keyIterInTimeBucket;
    };

    inline bool isExpired(time_point expiryTime) { return clock::now() > expiryTime };

    /* TimeBuckets. <expiryTime, list<keys that expire at expiryTime>  */
    std::map<time_point, std::list<Key>> timeBuckets;
    /* Holds iterator to cache and timeBuckets for each key
    Could have used pair<list<Node>::iterator, list<Key>::iterator> but
    created a struct for better readability
    */
    std::unordered_map<Key, IteratorsContainer> keyItersMap;

    std::mutex _mutex;
    size_t capacity;
};

#include "LRU.hpp"