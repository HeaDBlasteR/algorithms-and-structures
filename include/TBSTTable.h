#ifndef TBSTTABLE_H
#define TBSTTABLE_H

#pragma once
#include <memory>
#include <utility>
#include <stdexcept>
#include "TBinSearchTree.h"

template <typename Key, typename Value>
class TBSTTable {
private:
    TBinSearchTree<Key, Value> _data;

public:
    void insert(const Key& key, const Value& value) {
        _data.insert(key, value);
    }

    void remove(const Key& key) {
        _data.remove(key);
    }

    bool contains(const Key& key) const {
        return _data.contains(key);
    }

    Value& operator[](const Key& key) {
        if (!_data.contains(key)) {
            _data.insert(key, Value());
        }
        return _data.get(key);
    }

    const Value& operator[](const Key& key) const {
        return _data.get(key);
    }

    bool empty() const {
        return _data.empty();
    }
};

#endif // TBSTTABLE_H