// Copyright (c) 2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0

#include <erbsland/unittest/UnitTest.hpp>

#include <stdexcept>

SKIP_BY_DEFAULT()
class ConstructorFailureTest final : public el::UnitTest {
private:
    /// Throw when a fixture containing this member is constructed.
    class ThrowOnConstruction final {
    public:
        /// Always throw an exception.
        ThrowOnConstruction() { throw std::runtime_error{"constructor self-test exception"}; }
    };

public:
    SKIP_BY_DEFAULT()
    void testMustNotRun() { consoleWriteLine("CONSTRUCTOR_SENTINEL"); }

private:
    ThrowOnConstruction _throwOnConstruction{}; ///< The member that interrupts construction.
};
