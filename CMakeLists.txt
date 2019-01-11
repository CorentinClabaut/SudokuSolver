cmake_minimum_required (VERSION 3.8.2)

project (Sudoku)

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

add_executable(player
    ${SRCS}
)

target_link_libraries(player
    sudoku_lib
    pthread
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
    
