cmake_minimum_required(VERSION 2.8.3)
project(ZEBRA_ASSIGNMENT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-march=native -O3 -Wall")

add_library(plot_lib
             plot_lib/supportLib.cpp
             plot_lib/pbPlots.cpp)

add_executable(kdtree_search main.cpp utility.h plot_lib kdtree.cpp)
target_link_libraries(kdtree_search plot_lib)
