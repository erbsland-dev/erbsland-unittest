// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "Filter.hpp"

#include <set>
#include <string>
#include <vector>
#include <cstdint>


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

