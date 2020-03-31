cmake_minimum_required (VERSION 3.8.2)

project (Sudoku)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror")
set(CMAKE_CXX_COMPILER /usr/bin/g++-7)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost REQUIRED)
find_package(GTest REQUIRED)

include_directories("src/")

# Sudoku Solver Library

FILE(GLOB_RECURSE SRCS src/*.cpp)

add_library(sudoku_solver STATIC
    ${SRCS}
)

target_link_libraries(sudoku_solver
    pthread
    ${CONAN_LIBS}
)

# Benchmark Executable

FILE(GLOB_RECURSE SRCS_BENCHMARK benchmark/*.cpp)

add_executable(sudoku_solver_benchmark
    ${SRCS_BENCHMARK}
)

target_link_libraries(sudoku_solver_benchmark
    sudoku_solver
    profiler
)

# Test Executable

include_directories("test/")

FILE(GLOB_RECURSE TEST_SRCS test/*.cpp)

add_executable(sudoku_solver_tests
    ${TEST_SRCS}
)

target_link_libraries(sudoku_solver_tests
    ${CONAN_LIBS}
    sudoku_solver
    )
    
