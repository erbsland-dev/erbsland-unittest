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

