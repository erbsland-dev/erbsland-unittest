// Copyright © 2025 EducateIT GmbH (https://educateit.ch/) and Tobias Erbsland (https://erbsland.dev/)
// All rights are reserved. Detailed terms are specified in the "COPYRIGHT.md" file.
#pragma once


#include <cstdint>
#include <map>
#include <string>


namespace erbsland::unittest {


class ConsoleColor final {
public:
    enum Value : int8_t {
        Default = -1,
        Black = 0,
        DarkRed = 1,
        Green = 2,
        Orange = 3,
        DarkBlue = 4,
        Violet = 5,
        DarkCyan = 6,
        White = 7,
        DarkGray = 8,
        Red = 9,
        LightGreen = 10,
        Yellow = 11,
        LightBlue = 12,
        Magenta = 13,
        Cyan = 14,
        BrightWhite = 15,
    };

public:
    ConsoleColor() = default;
    constexpr ConsoleColor(const Value value) noexcept : _value{value} {} // NOLINT(*-explicit-constructor)

public:
    [[nodiscard]] constexpr auto operator==(const ConsoleColor &other) const noexcept -> bool {
        return _value == other._value;
    }
    [[nodiscard]] constexpr auto operator!=(const ConsoleColor &other) const noexcept -> bool {
        return _value != other._value;
    }

public:
    [[nodiscard]] constexpr auto value() const noexcept -> Value { return _value; }
    [[nodiscard]] auto foreground() const noexcept -> std::string;
    [[nodiscard]] auto background() const noexcept -> std::string;

private:
    static auto colorMap() noexcept -> const std::map<Value, std::tuple<int, int>>&;

private:
    Value _value{Default};
};


}

