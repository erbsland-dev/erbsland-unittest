// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later


#include <erbsland/unittest/UnitTest.hpp>

#include <ExampleLib.hpp>

#include <sstream>


using erbsland::ExampleLib;


TESTED_TARGETS(ExampleLib)
class ContextTest final : public el::UnitTest {
public:
    ExampleLib exampleLib{};

    auto additionalErrorMessages() -> std::string override {
        try {
            std::string text;
            text += "ExampleLib:\n";
            text += std::format("    getName() = \"{}\"\n", exampleLib.getName());
            text += std::format("    getNameLength() = {}\n", exampleLib.getNameLength());
            return text;
        } catch(...) {
            return {"Unexpected Exception"};
        }
    }

    void setAndVerifyName(const std::string &name) {
        exampleLib.setName(name);
        auto expectedSize = name.size();
        REQUIRE(exampleLib.getName() == name);
        REQUIRE(exampleLib.getNameLength() == expectedSize);
    }

    void setAndVerifyName_error(const std::string &name) {
        // This method just demonstrates what happens when an error occurs.
        exampleLib.setName(name);
        auto expectedSize = name.size();
        REQUIRE(exampleLib.getName() == name);
        REQUIRE_FALSE(exampleLib.getNameLength() == expectedSize);
    }

    TESTED_TARGETS(getName getNameLength setName)
    void testNameSetAndGet() {
        WITH_CONTEXT(setAndVerifyName({}));
        WITH_CONTEXT(setAndVerifyName("joe"));
        WITH_CONTEXT(setAndVerifyName("anna"));
        WITH_CONTEXT(setAndVerifyName("Joe"));
        WITH_CONTEXT(setAndVerifyName("Anna"));
        WITH_CONTEXT(setAndVerifyName("Peter"));
        WITH_CONTEXT(setAndVerifyName_error("Lisa"));
    }
};

