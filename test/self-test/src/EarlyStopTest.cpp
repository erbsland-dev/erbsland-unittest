// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0

#include <erbsland/unittest/UnitTest.hpp>

SKIP_BY_DEFAULT()
class EarlyStopTest final : public el::UnitTest {
public:
    SKIP_BY_DEFAULT()
    void testFailure() { REQUIRE(false); }

    SKIP_BY_DEFAULT()
    void testSentinel() { consoleWriteLine("EARLY_STOP_SENTINEL"); }
};
