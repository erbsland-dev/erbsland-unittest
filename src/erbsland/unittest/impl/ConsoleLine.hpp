// Copyright © 2025 EducateIT GmbH (https://educateit.ch/) and Tobias Erbsland (https://erbsland.dev/)
// All rights are reserved. Detailed terms are specified in the "COPYRIGHT.md" file.
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
    static auto utf8RemoveControlCharacters(const std::string_view &text) noexcept -> std::string;

private:
    std::vector<Part> _parts;
};


}

