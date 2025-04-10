#include "TBinSearchTree.h"
#include "TBSTTable.h"
#include <stdexcept>

template <typename Key, typename Value>
typename TBinSearchTree<Key, Value>::TNode* TBinSearchTree<Key, Value>::insert(
    TNode* node, const Key& key, const Value& value) {
    if (!node) {
        return new TNode(key, value);
    }

    if (key < node->key) {
        node->left = insert(node->left, key, value);
    }
    else if (key > node->key) {
        node->right = insert(node->right, key, value);
    }
    else {
        node->value = value; // ќбновление значени€ при совпадении ключей
    }

    return node;
}

template <typename Key, typename Value>
typename TBinSearchTree<Key, Value>::TNode* TBinSearchTree<Key, Value>::findMin(TNode* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename Key, typename Value>
typename TBinSearchTree<Key, Value>::TNode* TBinSearchTree<Key, Value>::remove(
    TNode* node, const Key& key) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->left = remove(node->left, key);
    }
    else if (key > node->key) {
        node->right = remove(node->right, key);
    }
    else {
        if (!node->left) {
            TNode* temp = node->right;
            delete node;
            return temp;
        }
        else if (!node->right) {
            TNode* temp = node->left;
            delete node;
            return temp;
        }

        TNode* minNode = findMin(node->right);
        node->key = minNode->key;
        node->value = minNode->value;
        node->right = remove(node->right, minNode->key);
    }

    return node;
}

template <typename Key, typename Value>
bool TBinSearchTree<Key, Value>::contains(TNode* node, const Key& key) const {
    if (!node) return false;

    if (key < node->key) {
        return contains(node->left, key);
    }
    else if (key > node->key) {
        return contains(node->right, key);
    }
    else {
        return true;
    }
}

template <typename Key, typename Value>
Value& TBinSearchTree<Key, Value>::get(TNode* node, const Key& key) const {
    if (!node) throw std::out_of_range("Key not found");

    if (key < node->key) {
        return get(node->left, key);
    }
    else if (key > node->key) {
        return get(node->right, key);
    }
    else {
        return node->value;
    }
}

// явные инстанциации дл€ распространЄнных типов
template class TBinSearchTree<int, int>;
template class TBinSearchTree<std::string, std::string>;