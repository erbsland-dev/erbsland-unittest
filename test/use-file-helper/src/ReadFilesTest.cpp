// Copyright © 2025 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later


#include <erbsland/unittest/UnitTest.hpp>
#include <erbsland/unittest/FileHelper.hpp>

#include <filesystem>


namespace fh = erbsland::unittest::fh;
namespace fs = std::filesystem;

class ReadFilesTest final : public el::UnitTest {
public:
    void testAccessTheExecutablePath() {
        auto path = fh::unitTestExecutablePath();
        REQUIRE(fs::exists(path));
        REQUIRE(fs::is_regular_file(path));
    }

    void testAccessTestData() {
        auto path = fh::resolveDataPath("data/test-data-file.txt");
        REQUIRE(fs::exists(path));
        REQUIRE(fs::is_regular_file(path));
    }

    void testReadTestData() {
        auto text = fh::readDataText("data/test-data-file.txt");
        REQUIRE(text.size() > 0);
        // Make sure CRLF->LF and UTF-8 on windows works.
        REQUIRE_EQUAL(text, "first line\nsecond line\n😀");
    }

    void testReadLines() {
        auto lines = fh::readDataLines("data/test-data-file.txt");
        REQUIRE(lines.size() == 3);
        REQUIRE_EQUAL(lines[0], "first line");
        REQUIRE_EQUAL(lines[1], "second line");
        REQUIRE_EQUAL(lines[2], "😀");
    }
};

