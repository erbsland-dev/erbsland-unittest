// Copyright (c) 2025-2026 Tobias Erbsland - https://erbsland.dev
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstdint>
#include <string>

namespace erbsland {

/// The interface of the example library.
///
/// This is a library to demonstrate *Erbsland Unit Test* features.
/// It deliberately contains problems to allow the unit test detecting issues.
///
class ExampleLib {
public:
    // defaults
    ExampleLib() = default;
    ~ExampleLib() = default;

public:
    /// Get the magic word.
    ///
    /// @return The magic word.
    ///
    [[nodiscard]] auto getMagicWord() const noexcept -> std::string;

    /// Set the name.
    ///
    /// @param name The new name.
    ///
    void setName(std::string name) noexcept;

    /// Get the name.
    ///
    /// @return The name.
    ///
    [[nodiscard]] auto getName() const noexcept -> std::string;

    /// Get the length of the current name.
    ///
    /// @return The length of the name in characters.
    ///
    [[nodiscard]] auto getNameLength() const noexcept -> std::size_t;

    /// Check if the name is a palindrome.
    ///
    /// @return `true` if the name is a palindrome.
    ///
    [[nodiscard]] auto isNamePalindrome() const noexcept -> bool;

    /// Check if the palindrome is an even or odd one.
    ///
    /// @return `true` if it is an even one (with 2, 4, 6, ... letters).
    /// @throws std::domain_error If the name is no palindrome.
    ///
    [[nodiscard]] auto isEvenPalindrome() const -> bool;

private:
    std::string _name;
};

}
