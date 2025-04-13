#include <iostream>
#include <vector>
#include <optional>
#include <list>
#include <utility>

template<typename K, typename V>
class OpenAddressingHashTable {
private:
    enum class EntryState {
        EMPTY, OCCUPIED, DELETED
    };

    struct Entry {
        K key;
        V value;
        EntryState state;

        Entry() : state(EntryState::EMPTY) {}
    };

    std::vector<Entry> table;
    size_t capacity;
    size_t size;
    const double load_factor_threshold = 0.7;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

    void rehash() {
        size_t new_capacity = capacity * 2;
        std::vector<Entry> new_table(new_capacity);

        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].state == EntryState::OCCUPIED) {
                size_t index = std::hash<K>{}(table[i].key) % new_capacity;

                while (new_table[index].state == EntryState::OCCUPIED) {
                    index = (index + 1) % new_capacity;
                }

                new_table[index].key = table[i].key;
                new_table[index].value = table[i].value;
                new_table[index].state = EntryState::OCCUPIED;
            }
        }

        table = std::move(new_table);
        capacity = new_capacity;
    }

public:
    OpenAddressingHashTable(size_t initial_capacity = 16)
        : capacity(initial_capacity), size(0) {
        table.resize(capacity);
    }

    bool insert(const K& key, const V& value) {
        if (static_cast<double>(size) / capacity >= load_factor_threshold) {
            rehash();
        }

        size_t index = hash(key);
        size_t start_index = index;
        size_t deleted_index = -1;

        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].value = value;
                return true;
            }
            else if (table[index].state == EntryState::DELETED && deleted_index == -1) {
                deleted_index = index;
            }
            else if (table[index].state == EntryState::EMPTY) {
                if (deleted_index != -1) {
                    index = deleted_index;
                }
                table[index].key = key;
                table[index].value = value;
                table[index].state = EntryState::OCCUPIED;
                size++;
                return true;
            }
            index = (index + 1) % capacity;
        } while (index != start_index);

        if (deleted_index != -1) {
            table[deleted_index].key = key;
            table[deleted_index].value = value;
            table[deleted_index].state = EntryState::OCCUPIED;
            size++;
            return true;
        }

        return false;
    }

    std::pair<bool, V> search(const K& key) const {
        size_t index = hash(key);
        size_t start_index = index;

        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                return { true, table[index].value };
            }
            else if (table[index].state == EntryState::EMPTY) {
                return { false, V() };
            }
            index = (index + 1) % capacity;
        } while (index != start_index);

        return { false, V() };
    }

    bool remove(const K& key) {
        size_t index = hash(key);
        size_t start_index = index;

        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].state = EntryState::DELETED;
                size--;
                return true;
            }
            else if (table[index].state == EntryState::EMPTY) {
                return false;
            }
            index = (index + 1) % capacity;
        } while (index != start_index);

        return false;
    }

    void print() const {
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].state == EntryState::OCCUPIED) {
                std::cout << "[" << i << "]: " << table[i].key << " -> " << table[i].value << std::endl;
            }
            else if (table[i].state == EntryState::DELETED) {
                std::cout << "[" << i << "]: DELETED" << std::endl;
            }
            else {
                std::cout << "[" << i << "]: EMPTY" << std::endl;
            }
        }
    }

    size_t get_size() const { return size; }
    size_t get_capacity() const { return capacity; }
};

template<typename K, typename V>
class ChainingHashTable {
private:
    struct Entry {
        K key;
        V value;

        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    std::vector<std::list<Entry>> table;
    size_t capacity;
    size_t size;
    const double load_factor_threshold = 0.7;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

    void rehash() {
        size_t new_capacity = capacity * 2;
        std::vector<std::list<Entry>> new_table(new_capacity);

        for (auto& chain : table) {
            for (auto& entry : chain) {
                size_t index = std::hash<K>{}(entry.key) % new_capacity;
                new_table[index].emplace_back(entry.key, entry.value);
            }
        }

        table = std::move(new_table);
        capacity = new_capacity;
    }

public:
    ChainingHashTable(size_t initial_capacity = 16)
        : capacity(initial_capacity), size(0) {
        table.resize(capacity);
    }

    bool insert(const K& key, const V& value) {
        if (static_cast<double>(size) / capacity >= load_factor_threshold) {
            rehash();
        }

        size_t index = hash(key);
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                entry.value = value;
                return true;
            }
        }

        table[index].emplace_back(key, value);
        size++;
        return true;
    }

    std::pair<bool, V> search(const K& key) const {
        size_t index = hash(key);
        for (const auto& entry : table[index]) {
            if (entry.key == key) {
                return { true, entry.value };
            }
        }
        return { false, V() };
    }

    bool remove(const K& key) {
        size_t index = hash(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                size--;
                return true;
            }
        }
        return false;
    }

    void print() const {
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "[" << i << "]: ";
            for (const auto& entry : table[i]) {
                std::cout << entry.key << " -> " << entry.value << "; ";
            }
            std::cout << std::endl;
        }
    }

    size_t get_size() const { return size; }
    size_t get_capacity() const { return capacity; }
};

template<typename HashTable>
HashTable mergeDictionaries(const std::vector<std::pair<std::string, int>>& table1,
    const std::vector<std::pair<std::string, int>>& table2,
    bool preferFirst) {
    HashTable mergedDict;

    // Сначала вставляем все элементы из предпочитаемой таблицы
    const auto& preferredTable = preferFirst ? table1 : table2;
    for (const auto& entry : preferredTable) {
        mergedDict.insert(entry.first, entry.second);
    }

    // Затем вставляем элементы из второй таблицы, пропуская дубликаты
    const auto& secondaryTable = preferFirst ? table2 : table1;
    for (const auto& entry : secondaryTable) {
        if (!mergedDict.search(entry.first).first) {
            mergedDict.insert(entry.first, entry.second);
        }
    }

    return mergedDict;
}

template<typename HashTable>
void printDictionary(const HashTable& dict, const std::string& title) {
    std::cout << title << ":\n";
    dict.print();
    std::cout << std::endl;
}

int main() {

    setlocale(LC_ALL, "Russian");

    std::vector<std::pair<std::string, int>> table1 = {
        {"абажур", 1},
        {"кинотеатр", 2},
        {"самолет", 3},
        {"человек", 4}
    };

    std::vector<std::pair<std::string, int>> table2 = {
        {"кинотеатр", 15},
        {"музыка", 16},
        {"самолет", 17}
    };

    {
        std::cout << "=== Тестирование OpenAddressingHashTable ===" << std::endl;

        auto dict1 = mergeDictionaries<OpenAddressingHashTable<std::string, int>>(table1, table2, true);
        printDictionary(dict1, "Объединенный словарь (приоритет table1)");

        auto dict2 = mergeDictionaries<OpenAddressingHashTable<std::string, int>>(table1, table2, false);
        printDictionary(dict2, "Объединенный словарь (приоритет table2)");
    }

    {
        std::cout << "=== Тестирование ChainingHashTable ===" << std::endl;

        auto dict1 = mergeDictionaries<ChainingHashTable<std::string, int>>(table1, table2, true);
        printDictionary(dict1, "Объединенный словарь (приоритет table1)");

        auto dict2 = mergeDictionaries<ChainingHashTable<std::string, int>>(table1, table2, false);
        printDictionary(dict2, "Объединенный словарь (приоритет table2)");
    }

    return 0;
}
