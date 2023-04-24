# Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
# Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
#
# This program is free software: you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with this program.
# If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.20)
include_guard()

# Add unittest metadata processing to the given target.
function(erbsland_unittest)
    # Read the arguments.
    set(options PRECOMPILE_HEADERS NO_LINK_SETTINGS)
    set(oneValueArgs TARGET)
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

    # Create a new target to build metadata for the unittest.
    cmake_path(SET _testMetaDataSrc NORMALIZE "${_targetBinaryDir}/unittest_metadata.cpp")
    add_custom_target("${ARGS_TARGET}MetaGen"
            "${Python3_EXECUTABLE}" "${_unittestScriptDir}/generate_unittest_metadata.py" "-v"
                "${_targetSourceDir}"
                "${_testMetaDataSrc}"
                "${_testSourcesPath}"
            DEPENDS "${_testSourcesPath}" ${_sourcesToProcess}
            BYPRODUCTS "${_testMetaDataSrc}"
            COMMAND_EXPAND_LISTS
            USES_TERMINAL
    )

    # Add the metadata target and file to the build process.
    add_dependencies("${ARGS_TARGET}" "${ARGS_TARGET}MetaGen")
    target_sources(${ARGS_TARGET} PRIVATE "${_testMetaDataSrc}")
endfunction()

