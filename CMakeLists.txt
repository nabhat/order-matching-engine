cmake_minimum_required(VERSION 3.20)
project(order_matching_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ome
    src/placeholder.cpp
)

target_include_directories(ome
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
