// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "MetaData.hpp"

#include <string>


namespace erbsland::unittest {


class UnitTest;


/// @internal
/// Base class for all registered tests.
///
class TestBase
{
public:
    /// ctor
    ///
    /// @param metaData Meta data of the test.
    ///
    explicit TestBase(MetaData metaData);

    /// dtor
    ///
    virtual ~TestBase() = default;

public:
    /// Access the metadata
    ///
    [[nodiscard]] auto metaData() const -> const MetaData&;

    /// Get the name of the test.
    ///
    [[nodiscard]] auto name() const -> const std::string&;

    /// Get the short name of the test.
    ///
    [[nodiscard]] auto shortName() const -> const std::string&;

    /// Call the actual test.
    ///
    virtual void call(UnitTest *unitTest) = 0;

    /// Test if this test is enabled.
    ///
    [[nodiscard]] auto isEnabled() const -> bool;

    /// Set if this test is enabled.
    ///
    void setEnabled(bool enabled);

private:
    MetaData _metaData; ///< Meta data of the test.
    bool _enabled; ///< If this test is enabled.
};


}

