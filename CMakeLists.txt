cmake_minimum_required(VERSION 3.15)
project(laba_2 VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Строгие предупреждения.
# Любое предупреждение считается ошибкой.
if(MSVC)
add_compile_options(
/W4
/WX
/utf-8
)
else()
add_compile_options(
-Wall
-Wextra
-Wpedantic
-Werror
)
endif()

# Подключаем Google Test автоматически.
include(FetchContent)

FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Настройка нужна только для компилятора Microsoft.
if(MSVC)
set(
        gtest_force_shared_crt
        ON
        CACHE BOOL ""
        FORCE
)
endif()

FetchContent_MakeAvailable(googletest)

# Основная консольная программа.
add_executable(
${PROJECT_NAME}
main.cpp
)

# Отдельная программа с модульными тестами.
add_executable(
${PROJECT_NAME}_tests
tests.cpp
)

# В tests.cpp подключаем gtest
target_link_libraries(
${PROJECT_NAME}_tests
PRIVATE
GTest::gtest
)

#разрешение cmake запускать тесты
enable_testing()
include(GoogleTest)

#находим все TEST() в tests.cpp
gtest_discover_tests(${PROJECT_NAME}_tests)
