cmake_minimum_required(VERSION 3.23)

project(unittest-basic)
add_executable(unittest-basic
        src/main.cpp
        src/BasicTest.cpp
        src/ContextTest.cpp
        src/LongTest.cpp
        src/TestHelper.hpp
        src/PriorityTest.cpp
)
target_compile_features(unittest-basic PRIVATE cxx_std_20)
target_link_libraries(unittest-basic PRIVATE mock-lib)
target_include_directories(unittest-basic PRIVATE ../mock-lib/src)
erbsland_unittest(TARGET unittest-basic ENABLE_WARNINGS)

