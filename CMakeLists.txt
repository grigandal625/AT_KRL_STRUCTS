cmake_minimum_required(VERSION 3.10)
project(AT_KRL_STRUCTS)

# Подключаем библиотеку Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Подключаем библиотеку libxml2
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

# Подключаем библиотеку jsoncpp
find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

# Включаем директории для заголовочных файлов
include_directories(include)

# Добавляем исходные файлы и файлы тестов
set(SOURCE_FILES
    src/kb_entity.cpp
    src/kb_type.cpp
    src/membership_function.cpp
    src/non_factor.cpp
    src/kb_value.cpp
    src/kb_reference.cpp
    src/kb_operation.cpp
)

set(TEST_FILES
    tests/kb_entity_tests.cpp
    tests/kb_type_tests.cpp
    tests/membership_function_tests.cpp
    tests/non_factor_tests.cpp
    tests/kb_value_tests.cpp
    tests/kb_reference_tests.cpp
    tests/kb_operation_tests.cpp
)

# Создаем исполняемый файл для тестов
add_executable(AT_KRL_STRUCTS ${SOURCE_FILES} ${TEST_FILES})

# Слинковываем с библиотеками Google Test, libxml2 и jsoncpp
target_link_libraries(AT_KRL_STRUCTS 
    ${GTEST_BOTH_LIBRARIES} 
    ${LIBXML2_LIBRARIES} 
    ${JSONCPP_LIBRARIES}
    pthread
)