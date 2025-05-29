# Copyright © 2023-2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.23)
include_guard()

# Add unittest metadata processing to the given target.
function(erbsland_unittest)
    # Read the arguments.
    set(options PRECOMPILE_HEADERS NO_LINK_SETTINGS)
    set(oneValueArgs TARGET COPY_TEST_DATA)
    set(multiValueArgs "")
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Python interpreter is required to process the metadata
    find_package(Python3 3.11 COMPONENTS Interpreter REQUIRED)

    # Collect all paths
    cmake_path(SET _unittestScriptDir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    cmake_path(SET _unittestDir NORMALIZE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../..")
    cmake_path(SET _unittestSrcDir NORMALIZE "${_unittestDir}/src")
    cmake_path(SET _unittestIncludeDir NORMALIZE "${_unittestDir}/include")
    get_target_property(_targetSources ${ARGS_TARGET} SOURCES)
    get_target_property(_targetSourceDir ${ARGS_TARGET} SOURCE_DIR)
    get_target_property(_targetBinaryDir ${ARGS_TARGET} BINARY_DIR)

    # Create a static library that will contain all metadata for the unittest.
    set(META_TARGET "${ARGS_TARGET}MetaGen")

    # Configure the unittest target to include the unittest headers, etc.
    add_dependencies(${ARGS_TARGET} erbsland-unittest)
    target_include_directories(${ARGS_TARGET} PRIVATE "${_unittestIncludeDir}")
    # Include the source directory, because the generated script uses this to include the found headers.
    target_include_directories(${ARGS_TARGET} PRIVATE "${_targetSourceDir}")
    if(ARGS_PRECOMPILE_HEADERS)
        target_precompile_headers(${ARGS_TARGET} PRIVATE "${_unittestIncludeDir}/erbsland/unittest/all.hpp")
    endif()

    if(NOT ARGS_NO_LINK_SETTINGS)
        # link the target to the unit test.
        target_link_libraries(${ARGS_TARGET} PRIVATE erbsland-unittest)
    endif()

    # Create a list with all files that are part of the unittest.
    cmake_path(SET _testSourcesPath NORMALIZE "${_targetBinaryDir}/unittest_test_files.toml")
    file(WRITE "${_testSourcesPath}" "# Erbsland UnitTest - Test Files\n")
    file(APPEND "${_testSourcesPath}" "test_files = [\n")
    unset(_sourcesToProcess)
    foreach(src ${_targetSources})
        if(src MATCHES "(Test.cpp|Test.hpp|Test.h)$")
            cmake_path(SET _targetPath "${src}")
            cmake_path(ABSOLUTE_PATH _targetPath BASE_DIRECTORY "${_targetSourceDir}" NORMALIZE)
            file(APPEND "${_testSourcesPath}" "    \"${_targetPath}\",\n")
            list(APPEND _sourcesToProcess "${_targetPath}")
        endif()
    endforeach()
    file(APPEND "${_testSourcesPath}" "]\n# EOF\n\n")

    # Group source and header files.
    foreach(src IN LISTS _sourcesToProcess)
        get_filename_component(basename "${src}" NAME_WE)
        list(APPEND test_group_${basename} "${src}")
        list(APPEND basenames_list "${basename}")
    endforeach()
    list(REMOVE_DUPLICATES basenames_list)

    set(metadata_outputs "")
    foreach(name IN LISTS basenames_list)
        set(out_cpp "${_targetBinaryDir}/${name}_metadata.cpp")
        set(group_name "test_group_${name}")
        add_custom_command(
                OUTPUT "${out_cpp}"
                COMMAND "${Python3_EXECUTABLE}"
                "${_unittestScriptDir}/generate_unittest_metadata.py"
                "${_targetSourceDir}"
                "${out_cpp}"
                ${${group_name}}
                DEPENDS ${${group_name}}
                COMMENT "Generating metadata for ${name}"
                VERBATIM
        )
        list(APPEND metadata_outputs "${out_meta}")
        target_sources(${ARGS_TARGET} PRIVATE "${out_cpp}")
    endforeach()

    if(ARGS_COPY_TEST_DATA)
        cmake_path(SET _dataSourceDir NORMALIZE "${CMAKE_CURRENT_LIST_DIR}/${ARGS_COPY_TEST_DATA}")
        cmake_path(SET _dataDestDir NORMALIZE "${_targetBinaryDir}/${ARGS_COPY_TEST_DATA}")
        if (${CMAKE_VERSION} VERSION_LESS "3.26")
            # For CMake <3.26 use the old "copy_directory" command.
            set(_copyCmd "copy_directory")
        else()
            # For CMake >=3.26 use the more efficient "copy_directory_if_different"
            set(_copyCmd "copy_directory_if_different")
        endif()
        add_custom_command(TARGET ${ARGS_TARGET} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E ${_copyCmd} "${_dataSourceDir}" "${_dataDestDir}"
                COMMENT "Copy test data..."
                VERBATIM)
    endif()
endfunction()

