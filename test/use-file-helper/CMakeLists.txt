cmake_minimum_required(VERSION 3.23)

project(unittest-file-helper)
add_executable(unittest-file-helper
        src/main.cpp
        src/ReadFilesTest.cpp
)
target_compile_features(unittest-file-helper PRIVATE cxx_std_20)
target_link_libraries(unittest-file-helper PRIVATE mock-lib)
target_include_directories(unittest-file-helper PRIVATE ../mock-lib/src)
erbsland_unittest(
        TARGET unittest-file-helper
        ENABLE_WARNINGS
        COPY_TEST_DATA data
)

