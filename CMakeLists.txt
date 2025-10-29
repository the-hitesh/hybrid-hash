cmake_minimum_required(VERSION 3.10)
project(hybrid_hash VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -O3)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(hybrid_demo src/main.cpp src/HybridHash.cpp src/cuckoo.cpp src/hopscotch.cpp src/robinhood.cpp src/overflow_stash.cpp)
# For tests, you'll add gtest later or use Catch2
