#include "TBinSearchTree.h"
#include "TBSTTable.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <random>
#include <chrono>

TEST(TBinSearchTreeTest, EmptyTree) {
    TBinSearchTree<int, std::string> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_FALSE(tree.contains(0));
}

TEST(TBinSearchTreeTest, InsertAndContains) {
    TBinSearchTree<int, std::string> tree;
    tree.insert(1, "one");
    tree.insert(2, "two");

    EXPECT_TRUE(tree.contains(1));
    EXPECT_EQ(tree.get(1), "one");
    EXPECT_TRUE(tree.contains(2));
    EXPECT_EQ(tree.get(2), "two");
}

TEST(TBinSearchTreeTest, RemoveOperations) {
    TBinSearchTree<int, std::string> tree;
    tree.insert(1, "one");
    tree.insert(2, "two");

    EXPECT_TRUE(tree.remove(1));
    EXPECT_FALSE(tree.contains(1));
    EXPECT_FALSE(tree.remove(1));
    EXPECT_TRUE(tree.remove(2));
    EXPECT_TRUE(tree.empty());
}

TEST(TBinSearchTreeTest, UpdateValue) {
    TBinSearchTree<int, std::string> tree;
    tree.insert(1, "initial");
    tree.insert(1, "updated");

    EXPECT_EQ(tree.get(1), "updated");
}

TEST(TBinSearchTreeTest, ExceptionHandling) {
    TBinSearchTree<int, std::string> tree;
    EXPECT_THROW(tree.get(1), std::out_of_range);

    tree.insert(1, "test");
    EXPECT_NO_THROW(tree.get(1));
}

TEST(TBinSearchTreeTest, ComplexScenario) {
    TBinSearchTree<int, std::string> tree;
    const int test_size = 100;

    // Массовая вставка
    for (int i = 0; i < test_size; ++i) {
        tree.insert(i, "val_" + std::to_string(i));
    }

    // Проверка содержимого
    for (int i = 0; i < test_size; ++i) {
        EXPECT_TRUE(tree.contains(i));
        EXPECT_EQ(tree.get(i), "val_" + std::to_string(i));
    }

    // Удаление каждого второго элемента
    for (int i = 0; i < test_size; i += 2) {
        EXPECT_TRUE(tree.remove(i));
    }

    // Проверка после удаления
    for (int i = 0; i < test_size; ++i) {
        EXPECT_EQ(tree.contains(i), i % 2 != 0);
    }
}

TEST(TBinSearchTreeTest, CustomTypes) {
    struct Data { int id; std::string name; };
    TBinSearchTree<int, Data> tree;

    tree.insert(1, Data{ 1, "first" });
    tree.insert(2, Data{ 2, "second" });

    EXPECT_EQ(tree.get(1).name, "first");
    EXPECT_EQ(tree.get(2).id, 2);
}

TEST(TBinSearchTreeTest, LargeDatasetRandomized) {
    TBinSearchTree<int, int> tree;
    const int count = 100000;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, count * 10);

    // Вставка случайных значений
    std::vector<int> keys;
    for (int i = 0; i < count; ++i) {
        int key = distrib(gen);
        keys.push_back(key);
        tree.insert(key, key * 2);
    }

    // Проверка всех значений
    for (int key : keys) {
        EXPECT_EQ(tree.get(key), key * 2);
    }

    // Дополнительная диагностика
    std::cout << "[ DIAGNOSTIC ] Inserted " << count
        << " random elements. Tree depth: " << tree.maxDepth() << "\n";
}

TEST(TBinSearchTreeTest, PerformanceInsertRandomized) {
    TBinSearchTree<int, int> tree;
    const int count = 100000;
    const int step = count / 10;  // Замеряем каждые 10%

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, count * 10);

    auto start_total = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= count; ++i) {
        int key = distrib(gen);
        tree.insert(key, key);

        // Промежуточные замеры
        if (i % step == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_total);
            std::cout << "[ PROGRESS ] Inserted " << i << " elements ("
                << (i * 100 / count) << "%), time: " << duration.count() << " ms\n";
        }
    }

    auto end_total = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_total - start_total);

    // Запись результатов для анализа
    ::testing::Test::RecordProperty("TotalInsertTimeMs", total_duration.count());
    ::testing::Test::RecordProperty("TreeDepth", tree.maxDepth());
    ::testing::Test::RecordProperty("ElementsCount", count);

    std::cout << "[ RESULT ] Total insert time: " << total_duration.count() << " ms\n"
        << "           Average time per insert: "
        << (total_duration.count() * 1000.0 / count) << " us\n"
        << "           Final tree depth: " << tree.maxDepth() << "\n";
}
