cmake_minimum_required(VERSION 3.16)
project(lcsc_part2)

set(CMAKE_CXX_STANDARD 20)

add_library(gal INTERFACE)  # this is a header-only library (no source files)
target_include_directories(gal INTERFACE include)

add_executable(max_int_problem app/max_int_problem/main.cpp)
target_link_libraries(max_int_problem gal)

add_executable(max_int_num_exp app/max_int_problem/int_max_numerical_experiments.cpp)
target_link_libraries(max_int_num_exp gal)