# Copyright (c) 2025-2026 Tobias Erbsland - https://erbsland.dev
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.23)

# Set up the installation.
install(TARGETS erbsland-unittest
        EXPORT erbsland-unittest-targets
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
)
install(DIRECTORY include/
        DESTINATION include
)
install(DIRECTORY src/erbsland/unittest
        DESTINATION src/erbsland/unittest
        FILES_MATCHING
        PATTERN "*.hpp"
)
install(EXPORT erbsland-unittest-targets
        FILE erbsland-unittest-targets.cmake
        NAMESPACE ErbslandDEV::
        DESTINATION lib/cmake/erbsland-unittest
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/erbsland-unittestConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/erbsland-unittestConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/erbsland-unittestConfig.cmake"
        INSTALL_DESTINATION lib/cmake/erbsland-unittest
)
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/erbsland-unittestConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/erbsland-unittestConfigVersion.cmake"
        DESTINATION lib/cmake/erbsland-unittest
)

