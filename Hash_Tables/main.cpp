#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <map>
#include <sstream>
#include <algorithm>
#include <cmath>

class Polynomial {
private:
    std::map<int, double> terms; // ключ - степень, значение - коэффициент

public:
    Polynomial() {}

    Polynomial(const std::map<int, double>& terms) : terms(terms) {}

    // Добавление монома
    void addTerm(int exponent, double coefficient) {
        if (coefficient != 0.0) {
            terms[exponent] += coefficient;
            if (terms[exponent] == 0.0) {
                terms.erase(exponent);
            }
        }
    }

    std::string toString() const {
        if (terms.empty()) {
            return "0";
        }

        std::stringstream ss;
        bool firstTerm = true;

        for (std::map<int, double>::const_reverse_iterator it = terms.rbegin(); it != terms.rend(); ++it) {
            int exponent = it->first;
            double coefficient = it->second;

            if (!firstTerm) {
                ss << (coefficient >= 0 ? " + " : " - ");
            }
            else if (coefficient < 0) {
                ss << "-";
            }

            double absCoeff = std::abs(coefficient);
            if (absCoeff != 1.0 || exponent == 0) {
                if (absCoeff == floor(absCoeff)) {
                    ss << static_cast<int>(absCoeff);
                }
                else {
                    ss << absCoeff;
                }
            }

            if (exponent > 0) {
                ss << "x";
                if (exponent > 1) {
                    ss << "^" << exponent;
                }
            }

            firstTerm = false;
        }

        return ss.str();
    }

    bool operator==(const Polynomial& other) const {
        return terms == other.terms;
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
        os << poly.toString();
        return os;
    }

    const std::map<int, double>& getTerms() const {
        return terms;
    }
};

namespace std {
    template<>
    struct hash<Polynomial> {
        size_t operator()(const Polynomial& poly) const {
            size_t h = 0;
            const std::map<int, double>& terms = poly.getTerms();
            for (std::map<int, double>::const_iterator it = terms.begin(); it != terms.end(); ++it) {
                size_t term_hash = hash<int>()(it->first);
                term_hash ^= hash<double>()(it->second) + 0x9e3779b9 + (term_hash << 6) + (term_hash >> 2);

                h ^= term_hash + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}

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

    // Тестирование со строками и числами
    std::vector<std::pair<std::string, int> > table1;
    table1.push_back(std::make_pair("абажур", 1));
    table1.push_back(std::make_pair("кинотеатр", 2));
    table1.push_back(std::make_pair("самолет", 3));
    table1.push_back(std::make_pair("человек", 4));

    std::vector<std::pair<std::string, int> > table2;
    table2.push_back(std::make_pair("кинотеатр", 15));
    table2.push_back(std::make_pair("музыка", 16));
    table2.push_back(std::make_pair("самолет", 17));

    {
        std::cout << "=== Тестирование OpenAddressingHashTable ===" << std::endl;

        OpenAddressingHashTable<std::string, int> dict1 =
            mergeDictionaries<OpenAddressingHashTable<std::string, int> >(table1, table2, true);
        printDictionary(dict1, "Объединенный словарь (приоритет table1)");

        OpenAddressingHashTable<std::string, int> dict2 =
            mergeDictionaries<OpenAddressingHashTable<std::string, int> >(table1, table2, false);
        printDictionary(dict2, "Объединенный словарь (приоритет table2)");
    }

    {
        std::cout << "=== Тестирование ChainingHashTable ===" << std::endl;

        ChainingHashTable<std::string, int> dict1 =
            mergeDictionaries<ChainingHashTable<std::string, int> >(table1, table2, true);
        printDictionary(dict1, "Объединенный словарь (приоритет table1)");

        ChainingHashTable<std::string, int> dict2 =
            mergeDictionaries<ChainingHashTable<std::string, int> >(table1, table2, false);
        printDictionary(dict2, "Объединенный словарь (приоритет table2)");
    }

    // Тестирование с полиномами
    {
        std::cout << "\n=== Тестирование хранения полиномов в OpenAddressingHashTable ===" << std::endl;

        OpenAddressingHashTable<std::string, Polynomial> polyTable;

        Polynomial p1;
        p1.addTerm(2, 3.0);
        p1.addTerm(1, -2.0);
        p1.addTerm(0, 5.0);

        Polynomial p2;
        p2.addTerm(3, 1.0);
        p2.addTerm(1, 4.0);

        polyTable.insert("Полином 1", p1);
        polyTable.insert("Полином 2", p2);

        std::cout << "Содержимое таблицы:" << std::endl;
        polyTable.print();

        std::pair<bool, Polynomial> searchResult = polyTable.search("Полином 1");
        if (searchResult.first) {
            std::cout << "\nНайден полином: " << searchResult.second.toString() << std::endl;
        }
    }

    {
        std::cout << "\n=== Тестирование хранения полиномов в ChainingHashTable ===" << std::endl;

        ChainingHashTable<Polynomial, std::string> polyTable;

        Polynomial p1;
        p1.addTerm(2, 3.0);
        p1.addTerm(1, -2.0);
        p1.addTerm(0, 5.0);

        Polynomial p2;
        p2.addTerm(3, 1.0);
        p2.addTerm(1, 4.0);

        polyTable.insert(p1, "Описание полинома 1");
        polyTable.insert(p2, "Описание полинома 2");

        std::cout << "Содержимое таблицы:" << std::endl;
        polyTable.print();

        std::pair<bool, std::string> searchResult = polyTable.search(p1);
        if (searchResult.first) {
            std::cout << "\nНайдено описание: " << searchResult.second
                << " для полинома " << p1.toString() << std::endl;
        }
    }

    return 0;
}