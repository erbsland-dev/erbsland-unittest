# Copyright (c) 2025-2026 Tobias Erbsland - https://erbsland.dev
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.23)

if(ERBSLAND_UNITTEST_ENABLE_TESTS)
    add_subdirectory(test)

    enable_testing()
    add_test(NAME unittest-self-test COMMAND $<TARGET_FILE:self-test> --no-color)

    add_test(NAME unittest-self-test-time-short COMMAND $<TARGET_FILE:self-test> --no-color -t name:Timing)
    set_tests_properties(unittest-self-test-time-short PROPERTIES
            PASS_REGULAR_EXPRESSION "Total Time: "
            FAIL_REGULAR_EXPRESSION "TIME STATISTICS"
    )

    add_test(NAME unittest-self-test-time-long COMMAND $<TARGET_FILE:self-test> --no-color --time-tests name:Timing)
    set_tests_properties(unittest-self-test-time-long PROPERTIES
            PASS_REGULAR_EXPRESSION "Total Time: "
            FAIL_REGULAR_EXPRESSION "TIME STATISTICS"
    )

    add_test(NAME unittest-self-test-statistics COMMAND $<TARGET_FILE:self-test> --no-color --time-stat name:Timing)
    set_tests_properties(unittest-self-test-statistics PROPERTIES
            PASS_REGULAR_EXPRESSION "TIME STATISTICS"
            FAIL_REGULAR_EXPRESSION "  Total Time: "
    )

    add_test(NAME unittest-self-test-combined
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:Timing)
    set_tests_properties(unittest-self-test-combined PROPERTIES PASS_REGULAR_EXPRESSION "TIME STATISTICS")

    add_test(NAME unittest-self-test-help COMMAND $<TARGET_FILE:self-test> --no-color --help)
    set_tests_properties(unittest-self-test-help PROPERTIES WILL_FAIL TRUE)
    add_test(NAME unittest-self-test-help-output COMMAND $<TARGET_FILE:self-test> --no-color --help)
    set_tests_properties(unittest-self-test-help-output PROPERTIES PASS_REGULAR_EXPRESSION "-t/--time-tests")

    add_test(NAME unittest-self-test-requirement
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:RequirementFailure)
    set_tests_properties(unittest-self-test-requirement PROPERTIES WILL_FAIL TRUE)
    add_test(NAME unittest-self-test-requirement-output
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:RequirementFailure)
    set_tests_properties(unittest-self-test-requirement-output PROPERTIES PASS_REGULAR_EXPRESSION "FAILED!")

    add_test(NAME unittest-self-test-check
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:CheckFailure)
    set_tests_properties(unittest-self-test-check PROPERTIES WILL_FAIL TRUE)
    add_test(NAME unittest-self-test-check-output
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:CheckFailure)
    set_tests_properties(unittest-self-test-check-output PROPERTIES PASS_REGULAR_EXPRESSION "CHECK_CONTINUED")

    add_test(NAME unittest-self-test-exception
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:Exception)
    set_tests_properties(unittest-self-test-exception PROPERTIES WILL_FAIL TRUE)
    add_test(NAME unittest-self-test-exception-output
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:Exception)
    set_tests_properties(unittest-self-test-exception-output PROPERTIES PASS_REGULAR_EXPRESSION "EXCEPTION!")

    add_test(NAME unittest-self-test-constructor
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:ConstructorFailure)
    set_tests_properties(unittest-self-test-constructor PROPERTIES WILL_FAIL TRUE)
    add_test(NAME unittest-self-test-constructor-output
            COMMAND $<TARGET_FILE:self-test> --no-color -t --time-stat name:ConstructorFailure)
    set_tests_properties(unittest-self-test-constructor-output PROPERTIES
            PASS_REGULAR_EXPRESSION "Exception while creating the unit test instance"
            FAIL_REGULAR_EXPRESSION "CONSTRUCTOR_SENTINEL"
    )

    add_test(NAME unittest-self-test-early-stop
            COMMAND $<TARGET_FILE:self-test> --no-color -e -t --time-stat name:EarlyStop)
    set_tests_properties(unittest-self-test-early-stop PROPERTIES WILL_FAIL TRUE)
    add_test(NAME unittest-self-test-early-stop-output
            COMMAND $<TARGET_FILE:self-test> --no-color -e -t --time-stat name:EarlyStop)
    set_tests_properties(unittest-self-test-early-stop-output PROPERTIES
            PASS_REGULAR_EXPRESSION "FAILED!"
            FAIL_REGULAR_EXPRESSION "EARLY_STOP_SENTINEL"
    )
endif()
