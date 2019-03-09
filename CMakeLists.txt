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

find_package(Boost REQUIRED)
find_package(GTest REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories("src/")

# Testable Lib

FILE(GLOB_RECURSE SRCS src/*.cpp)

add_library(sudoku_lib STATIC
    ${SRCS}
)

target_link_libraries(sudoku_lib
    pthread
)

# Executable

add_executable(sudoku_solver
    ${SRCS}
)

target_link_libraries(sudoku_solver
    sudoku_lib
    pthread
    profiler
)

# Test

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories("test/")

FILE(GLOB_RECURSE TEST_SRCS test/*.cpp)

add_executable(sudoku_test
    ${TEST_SRCS}
)

target_link_libraries(sudoku_test
    gmock
    gtest
    pthread
    sudoku_lib
    )
    
