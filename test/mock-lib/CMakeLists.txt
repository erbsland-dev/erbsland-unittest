cmake_minimum_required(VERSION 3.23)

project(mock-lib)
add_library(mock-lib STATIC
        src/ExampleLib.hpp
        src/ExampleLib.cpp
        src/ExamplePriority.hpp
)
target_compile_features(mock-lib PRIVATE cxx_std_20)
target_include_directories(mock-lib INTERFACE "${CMAKE_CURRENT_LIST_DIR}/src")
