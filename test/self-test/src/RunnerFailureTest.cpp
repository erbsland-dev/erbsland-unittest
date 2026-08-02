// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0

#include <erbsland/unittest/UnitTest.hpp>

#include <stdexcept>

SKIP_BY_DEFAULT()
class RunnerFailureTest final : public el::UnitTest {
public:
    SKIP_BY_DEFAULT()
    void testRequirementFailure() { REQUIRE(false); }

    SKIP_BY_DEFAULT()
    void testCheckFailure() {
        CHECK(false);
        consoleWriteLine("CHECK_CONTINUED");
    }

    SKIP_BY_DEFAULT()
    void testException() { throw std::runtime_error{"self-test exception"}; }

    SKIP_BY_DEFAULT()
    void testUnknownException() { throw 42; }
};
