// Copyright (c) 2023-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "TestBase.hpp"

namespace erbsland::unittest {

/// @internal
/// Test for a given derived unit test class.
/// @tparam T The derived unit test class.
template <class T>
class Test : public TestBase {
public:
    using TestFunction = void (T::*)(); ///< The member function pointer to the test method.

public:
    /// ctor
    /// @param testFunction The test function pointer.
    /// @param metaData Meta data of the test function.
    Test(TestFunction testFunction, MetaData metaData) : TestBase(std::move(metaData)), _testFunction(testFunction) {}

public:
    /// Access the test function.
    [[nodiscard]] inline auto testFunction() const -> TestFunction { return _testFunction; }

public: // Implement TestBase
    void call(UnitTest *unitTest) override {
        auto ptr = static_cast<T *>(unitTest);
        ptr->setUp();
        (ptr->*_testFunction)();
        ptr->tearDown();
    }

private:
    TestFunction _testFunction; ///< The test function.
};

}
