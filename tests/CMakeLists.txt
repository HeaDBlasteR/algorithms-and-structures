# Тесты для TBinSearchTree
add_executable(test_TBinSearchTree test_TBinSearchTree.cpp)
target_link_libraries(test_TBinSearchTree PRIVATE TBinSearchTree)

# Тесты для TBSTTable
add_executable(test_TBSTTable test_TBSTTable.cpp)
target_link_libraries(test_TBSTTable PRIVATE TBSTTable)

# Добавляем тесты в CTest
add_test(NAME TBinSearchTree COMMAND test_TBinSearchTree)
add_test(NAME TBSTTable COMMAND test_TBSTTable)