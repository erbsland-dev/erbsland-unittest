# Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later


cmake_minimum_required(VERSION 3.23)


if(ERBSLAND_UNITTEST_ENABLE_TESTS)
    # Add the test subdirectory.
    add_subdirectory(test)

    # Enable the example unit tests.
    enable_testing()
    add_test(
            NAME unittest-basic
            COMMAND $<TARGET_FILE:unittest-basic>
    )
    set_tests_properties(unittest-basic PROPERTIES
            WILL_FAIL TRUE
    )
    add_test(
            NAME unittest-file-helper
            COMMAND $<TARGET_FILE:unittest-file-helper>
    )
endif()

