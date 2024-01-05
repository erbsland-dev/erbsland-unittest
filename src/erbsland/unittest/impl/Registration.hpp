// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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

