// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include <cassert>
#include <compare>
#include <cstdint>
#include <string>
#include <format>


namespace erbsland {


/// A priority.
///
/// This is a class to demonstrate *Erbsland Unit Test* features.
/// It deliberately contains problems to allow the unit test detecting issues.
///
class ExamplePriority {
public:
    enum Name : uint8_t {
        Lowest = 0,
        Low,
        Medium,
        High,
        Highest,
    };

public:
    ExamplePriority(const Name name) noexcept : _name{name} {} // NOLINT(*-explicit-constructor)

public: // operators
    auto operator==(const ExamplePriority &other) const noexcept -> bool { return _name == other._name; }
    auto operator!=(const ExamplePriority &other) const noexcept -> bool { return _name == other._name; } // oops
    auto operator<(const ExamplePriority &other) const noexcept -> bool { return _name < other._name; }
    auto operator<=(const ExamplePriority &other) const noexcept -> bool { return _name <= other._name; }
    auto operator>(const ExamplePriority &other) const noexcept -> bool { return _name > other._name; }
    auto operator>=(const ExamplePriority &other) const noexcept -> bool { return _name >= other._name; }
    auto operator<=>(const ExamplePriority &other) const noexcept -> std::strong_ordering { return _name <=> other._name; }

public: // conversion.
    static auto fromString(const std::string &str) -> ExamplePriority {
        if (str == "Lowest") { return ExamplePriority{Lowest}; }
        if (str == "Low") { return ExamplePriority{Low}; }
        if (str == "Medium") { return ExamplePriority{Medium}; }
        if (str == "High") { return ExamplePriority{High}; }
        if (str == "Highest") { return ExamplePriority{Highest}; }
        throw std::invalid_argument("Invalid priority.");
    }
    [[nodiscard]] auto toString() const noexcept -> std::string {
        switch (_name) {
        case Lowest: return "Lowest";
        case Low: return "Low";
        case Medium: return "Medium";
        case High: return "High";
        case Highest: return "Highest";
        default:
            assert(false);
            return {};
        }
    }

private:
    Name _name;
};


}


template <>
struct std::formatter<erbsland::ExamplePriority> : std::formatter<std::string> {
    auto format(const erbsland::ExamplePriority value, format_context& ctx) const {
        return std::formatter<std::string>::format(value.toString(), ctx);
    }
};

