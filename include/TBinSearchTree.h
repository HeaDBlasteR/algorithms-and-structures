#ifndef TBINSEARCHTREE_H
#define TBINSEARCHTREE_H

#include <memory>
#include <utility>
#include <stdexcept>
#include <initializer_list>


template <typename Key, typename Value>
class TBinSearchTree {
private:

    struct TNode {
        Key key;
        Value value;
        std::unique_ptr<TNode> left;
        std::unique_ptr<TNode> right;

        TNode(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<TNode> root;

    // Итеративная версия поиска
    TNode* findNode(const Key& key) const {
        auto* current = root.get();
        while (current) {
            if (key < current->key) {
                current = current->left.get();
            }
            else if (key > current->key) {
                current = current->right.get();
            }
            else {
                return current;
            }
        }
        return nullptr;
    }

public:
    TBinSearchTree() = default;

    //конструктор от списка инициализации
    TBinSearchTree(std::initializer_list<std::pair<Key, Value>> init) {
        for (const auto& [key, value] : init) {
            insert(key, value);
        }
    }

    int maxDepth() const {
        std::function<int(const std::unique_ptr<TNode>&)> calculate =
            [&](const auto& node) {
            if (!node) return 0;
            return 1 + std::max(calculate(node->left), calculate(node->right));
            };
        return calculate(root);
    }

    void insert(const Key& key, const Value& value) {
        auto* current = &root;
        while (*current) {
            if (key < (*current)->key) {
                current = &(*current)->left;
            }
            else if (key > (*current)->key) {
                current = &(*current)->right;
            }
            else {
                (*current)->value = value; // Обновление значения
                return;
            }
        }
        *current = std::make_unique<TNode>(key, value);
    }

    bool remove(const Key& key) {
        auto* current = &root;
        while (*current) {
            if (key < (*current)->key) {
                current = &(*current)->left;
            }
            else if (key > (*current)->key) {
                current = &(*current)->right;
            }
            else {
                // Найден узел для удаления
                if (!(*current)->left) {
                    *current = std::move((*current)->right);
                }
                else if (!(*current)->right) {
                    *current = std::move((*current)->left);
                }
                else {
                    // У узла есть оба потомка
                    auto* minNode = (*current)->right.get();
                    while (minNode->left) {
                        minNode = minNode->left.get();
                    }
                    (*current)->key = minNode->key;
                    (*current)->value = minNode->value;
                    // Рекурсивно удаляем минимальный узел
                    auto temp = std::move((*current)->right);
                    *current = std::move(temp);
                    remove(minNode->key);
                }
                return true;
            }
        }
        return false;
    }

    bool contains(const Key& key) const {
        return findNode(key) != nullptr;
    }

    Value& get(const Key& key) {
        auto* node = findNode(key);
        if (!node) throw std::out_of_range("Key not found");
        return node->value;
    }

    const Value& get(const Key& key) const {
        auto* node = findNode(key);
        if (!node) throw std::out_of_range("Key not found");
        return node->value;
    }

    bool empty() const noexcept {
        return !root;
    }
};

#endif // TBINSEARCHTREE_H