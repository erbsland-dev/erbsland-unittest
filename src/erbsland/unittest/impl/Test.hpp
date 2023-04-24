// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
// Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/>.
#pragma once


#include "TestBase.hpp"


namespace erbsland::unittest {


/// @internal
/// Test for a given derived unit test class.
///
/// @tparam T The derived unit test class.
///
template<class T>
class Test : public TestBase
{
public:
    using TestFunction = void (T::*)(); ///< The member function pointer to the test method.

public:
    /// ctor
    ///
    /// @param testFunction The test function pointer.
    /// @param metaData Meta data of the test function.
    ///
    Test(TestFunction testFunction, MetaData metaData)
        : TestBase(std::move(metaData)), _testFunction(testFunction) {
    }

public:
    /// Access the test function.
    ///
    [[nodiscard]] inline auto testFunction() const -> TestFunction {
        return _testFunction;
    }

public: // Implement TestBase
    void call(UnitTest *unitTest) override {
        auto ptr = static_cast<T*>(unitTest);
        ptr->setUp();
        (ptr->*_testFunction)();
        ptr->tearDown();
    }

private:
    TestFunction _testFunction; ///< The test function.
};


}

