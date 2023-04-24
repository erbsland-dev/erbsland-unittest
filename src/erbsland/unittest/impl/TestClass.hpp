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


#include "Test.hpp"
#include "TestBase.hpp"
#include "TestClassBase.hpp"

#include <memory>


namespace erbsland::unittest {


/// @internal
/// A test class of a certain unittest class type.
///
template<typename T>
class TestClass : public TestClassBase {
public:
    /// ctor
    ///
    explicit TestClass(MetaData metaData) : TestClassBase(std::move(metaData)) {
    }

    /// dtor
    ///
    ~TestClass() override {
        delete _unitTest;
    }

public:
    /// Add a test to this test class.
    ///
    void addTest(typename Test<T>::TestFunction fn, const MetaData& metaData) {
        _tests.emplace_back(std::make_shared<Test<T>>(fn, metaData));
    }

    [[nodiscard]] auto testCount() const -> std::size_t override {
        return _tests.size();
    }

    [[nodiscard]] auto testMetaData(std::size_t index) const -> const MetaData& override {
        return _tests[index]->metaData();
    }

    [[nodiscard]] auto test(std::size_t index) const -> TestBase* override {
        return _tests[index].get();
    }

    void callTest(std::size_t index) override {
        if (!_unitTest) {
            createUnitTest();
        }
        _unitTest->setUp();
        (_unitTest->*(_tests[index]->testFunction()))();
        _unitTest->tearDown();
    }

    void createUnitTest() override {
        _unitTest = new T();
    }

    [[nodiscard]] auto isEnabled() const -> bool override {
        for (const auto &test : _tests) {
            if (test->isEnabled()) {
                return true;
            }
        }
        return false;
    }

    void setEnabled(bool enabled) override {
        for (auto &test : _tests) {
            if (enabled && test->metaData().isPrintMethod()) {
                continue; // do not enable print methods on suite basis.
            }
            test->setEnabled(enabled);
        }
    }

private:
    std::vector<std::shared_ptr<Test<T>>> _tests{}; ///< A list of tests in this class.
    T* _unitTest{}; ///< The local unittest instance.
};


}

