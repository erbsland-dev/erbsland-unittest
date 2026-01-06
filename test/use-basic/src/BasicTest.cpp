// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later


#include <erbsland/unittest/UnitTest.hpp>

#include <ExampleLib.hpp>


using erbsland::ExampleLib;


TESTED_TARGETS(ExampleLib)
class BasicTest final : public el::UnitTest {
public:
    TESTED_TARGETS(getMagicWord)
    void testMagic() {
        auto exampleLib = ExampleLib{};
        REQUIRE(exampleLib.getMagicWord() == "Magic");
    }

    TESTED_TARGETS(getName getNameLength setName)
    void testNameSetAndGet() {
        auto exampleLib = ExampleLib{};
        REQUIRE(exampleLib.getName().empty());
        REQUIRE(exampleLib.getNameLength() == 0);
        exampleLib.setName("Joe");
        REQUIRE(exampleLib.getName() == "Joe");
        REQUIRE(exampleLib.getNameLength() == 3);
        exampleLib.setName("Anna");
        REQUIRE(exampleLib.getName() == "Anna");
        REQUIRE(exampleLib.getNameLength() == 4);
    }

    TESTED_TARGETS(setName isNamePalindrome)
    void testIsNamePalindrome() {
        auto exampleLib = ExampleLib{};
        exampleLib.setName("joe");
        REQUIRE_FALSE(exampleLib.isNamePalindrome());
        exampleLib.setName("anna");
        REQUIRE(exampleLib.isNamePalindrome());
        exampleLib.setName("Joe");
        REQUIRE_FALSE(exampleLib.isNamePalindrome());
        exampleLib.setName("Anna");
        REQUIRE(exampleLib.isNamePalindrome());
        exampleLib.setName({});
        REQUIRE_FALSE(exampleLib.isNamePalindrome());
    }

    TESTED_TARGETS(setName isNamePalindrome)
    void testIsEvenPalindrome() {
        auto exampleLib = ExampleLib{};
        exampleLib.setName("joe");
        REQUIRE_THROWS(exampleLib.isEvenPalindrome());
        exampleLib.setName("anna");
        REQUIRE(exampleLib.isEvenPalindrome());
        exampleLib.setName("alila");
        REQUIRE_FALSE(exampleLib.isNamePalindrome());
    }

};

