# указывайте последнюю доступную вам версию CMake
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

# название проекта
project(Algorithms-and-Data-Structures)

# затем следует список инструкций для подключения проектов из подкаталогов

include(cmake/function.cmake)         # подхватываем функции, реализованные в файле function.cmake
                                      # для простоты мы объединили наборы команд для создания статической библиотеки
								      # и для создания исполняемого проекта в отдельные функции

add_subdirectory(lib_easy_example)    # подключаем дополнительный CMakeLists.txt из подкаталога с именем lib_easy_example
add_subdirectory(main)                # подключаем дополнительный CMakeLists.txt из подкаталога с именем main

option(BTEST "build test?" ON)        # указываем подключаем ли google-тесты (ON или YES) или нет (OFF или NO)

if(BTEST)                             # если тесты подключены
    add_subdirectory(gtest)           # подключаем дополнительный CMakeLists.txt из подкаталога с именем gtest
    add_subdirectory(tests)           # подключаем дополнительный CMakeLists.txt из подкаталога с именем test
endif()
