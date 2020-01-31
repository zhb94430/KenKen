cmake_minimum_required(VERSION 3.13)
project(KenKen)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

find_package(Z3 REQUIRED PATHS /Users/Peter/NonbrewableLibraries)
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Z3_FOUND: ${Z3_FOUND}")
message(STATUS "Found Z3 ${Z3_VERSION_STRING}")
message(STATUS "Z3_DIR: ${Z3_DIR}")

add_executable(KenKen main.cpp)

target_include_directories(KenKen PRIVATE ${Z3_CXX_INCLUDE_DIRS})
target_link_libraries(KenKen PRIVATE ${Z3_LIBRARIES})

add_executable(Examples example.cpp)

target_include_directories(Examples PRIVATE ${Z3_CXX_INCLUDE_DIRS})
target_link_libraries(Examples PRIVATE ${Z3_LIBRARIES})