cmake_minimum_required(VERSION 3.15)

project(order_matching_engine
    VERSION 0.1.0
    LANGUAGES CXX
)
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Prevent GoogleTest from overriding our compiler/linker options
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Library target ----
add_library(ome
    src/Order.cpp
    src/OrderIDGenerator.cpp
    src/placeholder.cpp
)

target_include_directories(ome
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# ---- Warnings ----
target_compile_options(ome
    PRIVATE
        -Wall
        -Wextra
        -Werror
)

# ---- Tests (placeholder for now) ----
enable_testing()
add_executable(ome_tests
    tests/placeholder_test.cpp
    tests/order_test.cpp
    tests/order_id_generator_test.cpp
)
target_link_libraries(ome_tests PRIVATE ome gtest_main)

include(GoogleTest)
gtest_discover_tests(ome_tests)

# ---- Benchmarks (placeholder) ----
add_executable(ome_bench
    benchmarks/placeholder_bench.cpp
)
target_link_libraries(ome_bench PRIVATE ome)
