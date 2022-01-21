cmake_minimum_required(VERSION 3.16)
project(lcsc_part2)

set(CMAKE_CXX_STANDARD 20)

add_library(gal INTERFACE)  # this is a header-only library (no source files)
target_include_directories(gal INTERFACE include)

add_executable(max_int_problem app/max_int_problem/main.cpp)
target_link_libraries(max_int_problem gal)

# Add library for graph code
add_library(graph_lib STATIC "src/graph.cpp" "src/randomGraph.cpp" "src/testGraphs.cpp")
target_include_directories(graph_lib PUBLIC include)

# Add executable for testing graph library and link to said library
add_executable(testGraphInsert app/testGraphInsert.cpp)
target_link_libraries(testGraphInsert graph_lib)

# Add executable for longest_path problem and link to graph_lib and gal
add_executable(longest_path_problem app/longest_path_problem/main.cpp)
target_link_libraries(longest_path_problem graph_lib gal)