# cmake -DCMAKE_BUILD_TYPE=Debug
# cmake -DCMAKE_BUILD_TYPE=Release

cmake_minimum_required(VERSION 2.8)
project (SquareRemover)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -d -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall")

file(GLOB SOURCE_FILES "./src/*.cpp")
file(GLOB HEADER_FILES "./include/*.hpp")

include_directories("./include")

include_directories("/usr/local/include")
include_directories("/usr/include")

find_library(ANT_LIBRARY ant)

add_executable(SquareRemover ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(SquareRemover ${ANT_LIBRARY})