#include "TBinSearchTree.h"
#include "TBSTTable.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <random>

TEST(TBSTTableTest, InsertAndContains) {
    TBSTTable<int, std::string> table;
    EXPECT_FALSE(table.contains(1));

    table.insert(1, "one");
    EXPECT_TRUE(table.contains(1));
    EXPECT_EQ(table[1], "one");

    table.insert(2, "two");
    EXPECT_TRUE(table.contains(2));
    EXPECT_EQ(table[2], "two");
}

TEST(TBSTTableTest, Remove) {
    TBSTTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");

    table.remove(1);
    EXPECT_FALSE(table.contains(1));
    EXPECT_TRUE(table.contains(2));

    table.remove(2);
    EXPECT_FALSE(table.contains(2));
    EXPECT_TRUE(table.empty());
}

TEST(TBSTTableTest, EmptyTable) {
    TBSTTable<int, std::string> table;
    EXPECT_TRUE(table.empty());
}

TEST(TBSTTableTest, OperatorBracket) {
    TBSTTable<int, std::string> table;

    table[1] = "one";
    EXPECT_TRUE(table.contains(1));
    EXPECT_EQ(table[1], "one");

    table[1] = "new one";
    EXPECT_EQ(table[1], "new one");

    // Проверка создания элемента при обращении
    EXPECT_FALSE(table.contains(2));
    std::string value = table[2];
    EXPECT_TRUE(table.contains(2));
    EXPECT_TRUE(value.empty());
}

TEST(TBSTTableTest, ConstOperatorBracket) {
    TBSTTable<int, std::string> table;
    table.insert(1, "one");

    const auto& constTable = table;
    EXPECT_EQ(constTable[1], "one");
    EXPECT_THROW(constTable[2], std::out_of_range);
}

TEST(TBSTTableTest, ComplexOperations) {
    TBSTTable<int, int> table;

    // Вставка значений
    for (int i = 0; i < 100; ++i) {
        table[i] = i * i;
    }

    // Проверка значений
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(table.contains(i));
        EXPECT_EQ(table[i], i * i);
    }

    // Удаление части элементов
    for (int i = 0; i < 100; i += 3) {
        table.remove(i);
    }

    // Проверка оставшихся элементов
    for (int i = 0; i < 100; ++i) {
        if (i % 3 == 0) {
            EXPECT_FALSE(table.contains(i));
        }
        else {
            EXPECT_TRUE(table.contains(i));
            EXPECT_EQ(table[i], i * i);
        }
    }
}