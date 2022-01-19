cmake_minimum_required(VERSION 3.21)
project(lcscGA)

set(CMAKE_CXX_STANDARD 20)

add_library(graph_lib STATIC src/graph.cpp include/randomGraph.h src/randomGraph.cpp)

add_executable(graphTest apps/testGraphInsert.cpp)
target_link_libraries(graphTest PUBLIC graph_lib)