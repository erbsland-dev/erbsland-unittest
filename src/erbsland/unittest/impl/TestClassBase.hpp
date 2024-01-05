// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "MetaData.hpp"


namespace erbsland::unittest {


class TestBase;


/// @internal
/// The base class for test classes
///
class TestClassBase {
public:
    /// ctor
    ///
    explicit TestClassBase(MetaData metaData);

    /// dtor
    ///
    virtual ~TestClassBase() = default;

public:
    /// Access the meta data of this test class.
    ///
    [[nodiscard]] auto metaData() const -> const MetaData&;

    /// Access the name of the test class.
    ///
    [[nodiscard]] auto name() const -> const std::string&;

    /// Get the short name for this test class.
    ///
    [[nodiscard]]auto shortName() const -> const std::string&;

public: // api
    /// Get the number of tests in this test class.
    ///
    [[nodiscard]] virtual auto testCount() const -> std::size_t = 0;

    /// Access the meta data of a test.
    ///
    [[nodiscard]] virtual auto testMetaData(std::size_t index) const -> const MetaData& = 0;

    /// Call the test function for a test.
    ///
    virtual void callTest(std::size_t index) = 0;

    /// Access a test.
    ///
    [[nodiscard]] virtual auto test(std::size_t index) const -> TestBase* = 0;

    /// Create the unittest instance (internally).
    ///
    virtual void createUnitTest() = 0;

    /// Test if this class is enabled.
    ///
    [[nodiscard]] virtual auto isEnabled() const -> bool = 0;

    /// Enable/disable all tests in this class.
    ///
    virtual void setEnabled(bool enabled) = 0;

private:
    MetaData _metaData; ///< Meta data of the test class.
};


}

