// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later


#include <erbsland/unittest/UnitTest.hpp>

#include <ExampleLib.hpp>

#include <string>
#include <sstream>


using erbsland::ExampleLib;


TESTED_TARGETS(ExampleLib)
class LongTest final : public el::UnitTest {
public:
    static constexpr std::size_t size = 6;

    bool result{};
    std::string name;

    auto additionalErrorMessages() -> std::string override {
        try {
            std::string text;
            text += std::format("result = {}\n", result);
            text += std::format("name = \"{}\"\n", name);
            text += std::format("expected result = {}\n", expectedResult());
            return text;
        } catch(...) {
            return {"Unexpected Exception"};
        }
    }

    [[nodiscard]] auto expectedResult() noexcept -> bool {
        for (std::size_t i = 0; i < size/2; ++i) {
            if (name[i] != name[size - i - 1]) {
                return false;
            }
        }
        return true;
    }

    TAGS(long-test)
    SKIP_BY_DEFAULT()
    TESTED_TARGETS(setName isNamePalindrome)
    void testIsNamePalindromeBruteForce() {
        auto exampleLib = ExampleLib{};
        name = std::string(size, 'a');
        std::size_t count{0};
        while (name[size-1] != 'z') {
            exampleLib.setName(name);
            result = exampleLib.isNamePalindrome();
            REQUIRE(result == expectedResult());
            for (std::size_t i = 0; i < size; ++i) {
                if (name[i] < 'z') {
                    name[i] += 1;
                    break;
                } else {
                    name[i] = 'a';
                }
            }
            if (count % 1'000'000 == 0) {
                auto text = std::ostringstream{};
                text << "Testing name " << count << ": " << name;
                consoleWriteLine(text.str());
            }
            count += 1;
        }
    }
};

