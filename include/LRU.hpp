
template <typename Key, typename Value>
bool LRU<Key, Value>::get(const Key &key, const Value &value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    /* Key in map */
    if (keyItersMap.find(key) == keyItersMap.end())
        return false;
    IteratorsContainer temp = keyItersMap[key];
    auto expiryTime = temp.cacheIterator->expiryTime;

    /* Expiry time */
    if (isExpired(expiryTime))
        return false;

    value = temp.cacheIter->value;
    return true;
}

template <typename Key, typename Value>
void LRU<Key, Value>::set(const Key &key, const Value &value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (keyItersMap.find(key) != keyItersMap.end())
        return;
    if (keyItersMap.size() == capacity)
        /* handle a negative capacity */
        return;
    // auto cacheIter = Node(key, value);
    // auto keyIterInTimeBucket = key;
    // keyItersMap[key] = IteratorsContainer(cacheIter, keyIterInTimeBucket);

    timeBuckets[clock::now()].push_front(key);
}