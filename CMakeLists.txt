cmake_minimum_required(VERSION 3.30.3)
project(bst_table LANGUAGES CXX)

# Указываем путь к vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/Users/nozdr/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Настройка Google Test
find_package(GTest REQUIRED)

# Общие настройки
set(COMMON_INCLUDES 
    include
    ${GTEST_INCLUDE_DIRS}
)

# Библиотека бинарного дерева поиска (header-only)
add_library(TBinSearchTree INTERFACE)
target_include_directories(TBinSearchTree INTERFACE ${COMMON_INCLUDES})
target_sources(TBinSearchTree INTERFACE include/TBinSearchTree.h)

# Библиотека таблицы на основе дерева (header-only)
add_library(TBSTTable INTERFACE)
target_include_directories(TBSTTable INTERFACE ${COMMON_INCLUDES})
target_sources(TBSTTable INTERFACE include/TBSTTable.h)

# Включаем тесты
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    
    # Тесты для TBinSearchTree
    add_executable(test_TBinSearchTree tests/test_TBinSearchTree.cpp)
    target_link_libraries(test_TBinSearchTree
        TBinSearchTree
        GTest::GTest 
        GTest::Main
    )
    
    # Тесты для TBSTTable
    add_executable(test_TBSTTable tests/test_TBSTTable.cpp)
    target_link_libraries(test_TBSTTable
        TBSTTable
        GTest::GTest 
        GTest::Main
    )
    
    add_test(NAME test_TBinSearchTree COMMAND test_TBinSearchTree)
    add_test(NAME test_TBSTTable COMMAND test_TBSTTable)
endif()