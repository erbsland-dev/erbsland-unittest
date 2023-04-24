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


#include "Filter.hpp"

#include <set>
#include <string>
#include <vector>


namespace erbsland::unittest {


/// @internal
/// Metadata for test methods and testclasses.
///
class MetaData {
public:
    enum Flag : uint32_t {
        /// Skip this class/method by default.
        ///
        SkipByDefault = (1u<<1u),

        /// This is a print only method, used to visualize results while debugging.
        ///
        PrintMethod = (1u<<2u),
    };
    using Flags = uint32_t;

public:
    /// Create metadata with only the object name.
    ///
    explicit MetaData(const std::string& name);

    /// Create metadata with the object name, tags and targets.
    ///
    MetaData(
        const std::string &name,
        const std::vector<std::string> &tags,
        const std::vector<std::string> &testedTargets,
        Flags flags) noexcept;

public:
    /// Test if this matches the given filter option.
    ///
    /// @param filter The filter to test.
    /// @param option The option in the filter to test.
    /// @return `true` if there is a match.
    ///
    [[nodiscard]] auto matches(const Filter &filter, FilterOption option) const noexcept -> bool;

    /// The name.
    ///
    [[nodiscard]] auto name() const noexcept -> const std::string&;

    /// The short name.
    ///
    [[nodiscard]] auto shortName() const noexcept -> const std::string&;

    /// The tags.
    ///
    [[nodiscard]] auto tags() const noexcept -> const std::set<std::string>&;

    /// The targets.
    ///
    [[nodiscard]] auto targets() const noexcept -> const std::set<std::string>&;

    /// If the target shall be skipped by default.
    ///
    [[nodiscard]] auto isSkipByDefault() const noexcept -> bool;

    /// If the target is a print method.
    ///
    [[nodiscard]] auto isPrintMethod() const noexcept -> bool;

private:
    std::string _name; ///< The name.
    std::string _shortName; ///< A short version of the name.
    std::set<std::string> _tags; ///< The tags.
    std::set<std::string> _targets; ///< The targets.
    Flags _flags; ///< Flags.
};


}

