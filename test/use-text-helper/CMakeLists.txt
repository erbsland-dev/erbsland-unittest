cmake_minimum_required(VERSION 3.23)

project(unittest-text-helper)
add_executable(unittest-text-helper
        src/main.cpp
        src/TextHelperTest.cpp
)
target_compile_features(unittest-text-helper PRIVATE cxx_std_20)
target_link_libraries(unittest-text-helper PRIVATE mock-lib)
target_include_directories(unittest-text-helper PRIVATE ../mock-lib/src)
erbsland_unittest(
        TARGET unittest-text-helper
        ENABLE_WARNINGS
)

