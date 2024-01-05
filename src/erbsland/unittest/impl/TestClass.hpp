// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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

