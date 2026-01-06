// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "ExampleLib.hpp"


#include <stdexcept>


namespace erbsland {


auto ExampleLib::getMagicWord() const noexcept -> std::string {
    return "Magic";
}


void ExampleLib::setName(std::string name) noexcept {
    _name = std::move(name);
}


auto ExampleLib::getName() const noexcept -> std::string {
    return _name;
}


auto ExampleLib::getNameLength() const noexcept -> std::size_t {
    return _name.size();
}


auto ExampleLib::isNamePalindrome() const noexcept -> bool {
    // error 1: Does not handle empty strings.
    // error 2: Does not use case-insensitive comparison.
    // error 3: Does not handle space seperated names (that count as palindrome as well).
    const auto size = _name.size();
    for (std::size_t i = 0; i < size/2; ++i) {
        if (_name[i] != _name[size - i - 1]) {
            return false;
        }
    }
    return true;
}


auto ExampleLib::isEvenPalindrome() const -> bool {
    if (!isNamePalindrome()) {
        throw std::domain_error("The name is no palindrome.");
    }
    return getNameLength() % 2 == 0;
}


}

