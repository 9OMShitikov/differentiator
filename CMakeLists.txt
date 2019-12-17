cmake_minimum_required(VERSION 3.10)



project(read_expression)
set(CMAKE_CXX_STANDARD 17)
include_directories("auxilary_functions")
add_executable(read_expression main_files/reader.cpp)

project(count_expression)
set(CMAKE_CXX_STANDARD 17)
include_directories("auxilary_functions")
add_executable(count_expression main_files/count_tree.cpp)


project(print_tree)
set(CMAKE_CXX_STANDARD 17)
include_directories("auxilary_functions")
add_executable(print_tree main_files/print_tree.cpp)

project(differentiate_tree)
set(CMAKE_CXX_STANDARD 17)
include_directories("auxilary_functions")
add_executable(differentiate_tree main_files/differentiator.cpp)

project(simplify_tree)
set(CMAKE_CXX_STANDARD 17)
include_directories("auxilary_functions")
add_executable(simplify_tree main_files/simplifier.cpp)