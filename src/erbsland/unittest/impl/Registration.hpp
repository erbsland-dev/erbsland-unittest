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


#include "Controller.hpp"
#include "MetaData.hpp"
#include "TestClass.hpp"

#include <string>
#include <tuple>
#include <type_traits>
#include <vector>


namespace erbsland::unittest {


/// @internal
/// A helper class to automatically register unit test classes.
///
template<class T>
class Registration
{
public:
    explicit Registration(
        MetaData metaData,
        std::vector<std::tuple<void (T::*)(), MetaData>> testMethods = {}) noexcept {
        auto testClass = new TestClass<T>(metaData);
        for (const auto &[fn, md] : testMethods) {
            testClass->addTest(fn, md);
        }
        Controller::instance()->addTestClass(testClass);
    }
};


/// @internal
/// A helper class to automatically register unit test classes.
///
template<class T>
class ManualRegistration
{
public:
    explicit ManualRegistration(const char *className) noexcept {
        auto testClass = new TestClass<T>(MetaData{std::string{className}});
        for (const auto &[fn, name] : T::cTestFunctions) {
            testClass->addTest(fn, MetaData{std::string{name}});
        }
        Controller::instance()->addTestClass(testClass);
    }
};


}

