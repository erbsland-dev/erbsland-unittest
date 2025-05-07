// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "ConsoleColor.hpp"

#include <string>
#include <vector>


namespace erbsland::unittest {


/// A class to build a console line using colors
///
class ConsoleLine {
public:
    struct Part {
        std::string text;
        ConsoleColor foreground;
        ConsoleColor background;
    };

public:
    /// Create an empty line.
    ///
    ConsoleLine() = default;

    /// Create a simple line.
    ///
    /// @param text The text of the line.
    /// @param foreground The foreground color.
    /// @param background The background color.
    ///
    explicit ConsoleLine(const std::string &text, ConsoleColor foreground = {}, ConsoleColor background = {}) noexcept;

public:
    /// Test if this line is empty.
    ///
    [[nodiscard]] auto empty() const noexcept -> bool;

    /// Get the length of the text in characters.
    ///
    [[nodiscard]] auto length() const noexcept -> std::size_t;

    /// Get the individual coloured parts of the text.
    ///
    [[nodiscard]] auto parts() const noexcept -> const std::vector<Part>&;

    /// Add a new text part to the line.
    ///
    void addText(const std::string &text, ConsoleColor foreground = {}, ConsoleColor background = {}) noexcept;

public:
    /// Get the length of a UTF-8 formatted text.
    ///
    /// @param text The text.
    /// @return The number of characters.
    ///
    [[nodiscard]] static auto utf8Length(const std::string_view &text) noexcept -> std::size_t;

    /// Right justify a number.
    ///
    template<typename T>
    [[nodiscard]] static auto rightJustify(T number, std::size_t width) noexcept -> std::string {
        std::string result;
        result.reserve(width);
        auto numberStr = std::to_string(number);
        result.append(width - numberStr.size(), ' ');
        result += numberStr;
        return result;
    }

    /// Remove control characters from UTF-8 text.
    ///
    /// @param text The text to process.
    /// @return A text without any control characters.
    ///
    static auto utf8RemoveControlCharacters(const std::string_view &text) noexcept -> std::string;

    /// Create a safe string by escaping and length limiting the text.
    ///
    /// @param text The text to process.
    /// @param maxLength The maximum length of the *processed* text.
    /// @return A safe, escaped text that can be written to the console.
    ///
    static auto utf8SafeString(const std::string_view &text, std::size_t maxLength) noexcept -> std::string;

private:
    std::vector<Part> _parts;
};


}

