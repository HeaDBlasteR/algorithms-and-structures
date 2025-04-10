#include "TBSTTable.h"
#include "TBinSearchTree.h"
#include <stdexcept>

template <typename Key, typename Value>
void TBSTTable<Key, Value>::insert(const Key& key, const Value& value) {
    _data.insert(key, value);
}

template <typename Key, typename Value>
void TBSTTable<Key, Value>::remove(const Key& key) {
    _data.remove(key);
}

template <typename Key, typename Value>
bool TBSTTable<Key, Value>::contains(const Key& key) const {
    return _data.contains(key);
}

template <typename Key, typename Value>
Value& TBSTTable<Key, Value>::operator[](const Key& key) {
    if (!_data.contains(key)) {
        _data.insert(key, Value());
    }
    return _data.get(key);
}

template <typename Key, typename Value>
const Value& TBSTTable<Key, Value>::operator[](const Key& key) const {
    return _data.get(key);
}

template <typename Key, typename Value>
bool TBSTTable<Key, Value>::empty() const {
    return _data.empty();
}

// явные инстанциации дл€ распространЄнных типов
template class TBSTTable<int, int>;
template class TBSTTable<std::string, std::string>;