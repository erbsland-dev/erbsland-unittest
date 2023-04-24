# The Erbsland Unit Testing System

Welcome to *Erbsland UnitTest*, a standalone unit testing system designed for testing Erbsland libraries and applications. Our unit testing system is built with a focus on minimal dependencies, making it theoretically dependency-free. For a more convenient usage, it requires only a minimal set of dependencies (C++, CMake, Python) and is compatible with any C++17 or later compiler.

## Goals

- Dependency-Free
- User-Friendly
- Minimalistic
- Modern
- Secure

### Dependency-Free

In complex projects, we have often found that unit test dependencies can create conflicts with the targets being tested. To avoid such issues, we have developed a C++ unit testing system that relies solely on a moderately modern C++ compiler and its standard library, with no additional libraries required.

For a more convenient user experience, the CMake build system and Python are required. These tools automatically generate the necessary code to register test classes, functions, and their metadata. While writing unit tests without CMake and Python is not our primary focus, it is theoretically possible to do all the registration manually.

### User-Friendly

Writing unit tests is a crucial part of developing secure and reliable software, but it can be a tedious and time-consuming task. With this in mind, we designed our unit test system to minimize setup time and reduce the required files and code structures. A minimal working unit test only requires a `CMakeLists.txt`, `main.cpp`, and `ExampleTest.hpp` file. See the examples later in this document.

You can quickly expand your tests by adding tags, target information, and additional error messages. This allows you to easily scale from a minimal working test to a comprehensive test suite.

### Minimalistic

We believe that unit tests with an excessive number of macros and test functions do not necessarily result in better tests; instead, they can make the unit tests overly complex. Our unit test system adopts a minimalistic approach, providing a handful of test macros that cover most use cases while maintaining easy extensibility.

### Modern

Our unit test system is written in modern C++ (up to C++17) but also works with the latest C++20 features if needed. It is compatible with a wide range of compilers, even those that may not yet fully support C++17 features.

### Secure

Unit tests are often run automatically on build servers. Our unit test system is designed conservatively with security in mind, ensuring a minimal attack surface. The dependency-free architecture makes it easy to verify all execution paths, provided there are no serious flaws in the C++ standard library being used.

## Requirements

- C++ compiler that understands C++17 or newer.
- The C++ standard library supporting the feature set of C++17.
- CMake version 3.25 or newer (optional, see "Dependency Free" above).
- Python 3.11 or newer for the metadata compiler (optional, see "Dependency Free" above).

## Project Structure

*Erbsland UnitTest* is intended to be utilized as a GIT submodule within a larger project. To ensure seamless integration and easy usage, we recommend the following project structure for a project called `Example`:

```
ExampleUnitTest
    ├── <libraries>         # All dependent libraries for the `Example` project (e.g., as submodules)
    ├── erbsland-unittest   # The Erbsland UnitTest library as a submodule
    ├── Example             # The "Example" project to be tested by the unit test
    │   ├── src (...)
    │   ├── data
    │   └── CMakeLists.txt
    ├── UnitTest            # The actual unit test, containing one or more unit test binaries
    │   ├── src             # The source files for the unit tests
    │   │   ├── main.cpp           # The main file for the executable
    │   │   ├── BarTest.hpp        # The first test class (Note: this is a header file!)
    │   │   └── FooTest.hpp        # The second test class, with one class/file for each functionality
    │   ├── data            # Data for unit tests
    │   └── CMakeLists.txt
    └── CMakeLists.txt      # The root project CMake file
```

By following this structure, you can ensure that your project is well-organized and easy to navigate. It also makes it simple to manage your unit tests, ensuring efficient and thorough testing of your libraries and applications.

### Configuring the Root Project CMake File

To set up the root project CMake file, use the following template:

```CMake
cmake_minimum_required(VERSION 3.20)
project(ExampleUnitTest)
# ... add libraries ...
add_subdirectory(erbsland-unittest)
add_subdirectory(Example)  # Include this line if it's applicable for the tests
add_subdirectory(UnitTest)
```

This configuration ensures that the project properly includes all necessary subdirectories for building and testing the `Example` project.

### Setting Up the UnitTest CMake File

For a simple unit test with one executable, as illustrated in the example project structure, you'll need a CMake file like the one shown below:

```CMake
cmake_minimum_required(VERSION 3.20)
project(UnitTestProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(UnitTest)
add_subdirectory(src)
erbsland_unittest(TARGET UnitTest)
```

This configuration sets up the UnitTest project with C++17 as the standard and requires it for compilation. It also adds the `src` subdirectory and incorporates the Erbsland UnitTest system.

Next, create a CMake file within the `src` subdirectory, using the following template:

```CMake
target_sources(UnitTest PRIVATE
    main.cpp
    BarTest.hpp
    FooTest.hpp)
```

This configuration specifies the source files for the unit test, ensuring that the test classes are properly included and executed.

## Crafting the `main.cpp` File

The `main.cpp` file serves as the entry point for your unit tests. To set it up, follow the example below:

```c++
#include <erbsland/unittest/UnitTest.hpp>
ERBSLAND_UNITTEST_MAIN();
```

By using the `ERBSLAND_UNITTEST_MAIN();` macro, you can quickly and easily create the main function for your unit test executable.

## Utilizing the Test API

Working with the Erbsland UnitTest API is straightforward. For each set of tests, create a `<name>Test.hpp` file and declare a `<name>Test` class in the header file. This class should inherit from `erbsland::UnitTest`.

At a minimum, your `<name>Test` class should contain one or more public methods that begin with the word `test`.

Below is an example of a `BarTest.hpp` file:

```c++
#pragma once
#include <example/Bar.hpp>
#include <erbsland/unittest/UnitTest.hpp>

class BarTest : public erbsland::UnitTest {
public:
    void testBar() {
        Bar bar;
        REQUIRE(bar.isOk());
        REQUIRE_FALSE(bar.value() == 7);
    }
};
```

With this setup, the built-in metadata compiler will take care of the rest.

### Naming Requirements

To ensure consistency and compatibility, adhere to the following naming requirements:

- Test cases should be placed in header files with names ending in `...Test.hpp`.
- The header file should contain a `class` with the exact same name as the file. For example, a file named `FooTest.hpp` should include a class definition `class FooTest : public erbsland::UnitTest { ... };`.
- The class does not need a constructor, but it should contain public methods in the form `void test<name>() { ... }`. It's important that the method names start with the lowercase `test`. All methods that begin with `test` are automatically called in the order they are defined. You can declare any number of additional methods that do not start with `test`.

### Test Macros

Erbsland UnitTest supports the following test macros:

- `REQUIRE(expression)`: Tests if the given expression is true.
- `REQUIRE_FALSE(expression)`: Tests if the given expression is false.
- `REQUIRE_THROWS(expression)`: Tests if the given expression throws an exception. Fails if it does not.
- `REQUIRE_THROWS_AS(<exception class>, expression)`: Tests if the given expression throws an exception of the specified type or derived type `<exception class>`. Fails if it does not.
- `REQUIRE_NOTHROW(expression)`: Tests if the given expression does *not* throw an exception. Fails if it does.

For all `REQUIRE_...` macros listed above, there is a corresponding `CHECK_...` version. These check versions only display a message in the output, but the test will not fail.

- `CHECK`
- `CHECK_FALSE`
- `CHECK_THROWS`
- `CHECK_THROWS_AS`
- `CHECK_NOTHROW`

### Running Code Before and After Each Test

To execute code before and after each test function, you can override the classic `setUp()` and `tearDown()` methods:

```c++
class ExampleTest : public erbsland::UnitTest {
public:
    void setUp() override {
        // ...
    }
    void tearDown() override {
        // ...
    }
    // ...
};
```

### Providing Custom Error Details

To gain more insight into the data used when a test fails, you can provide additional custom error details. The example below demonstrates the use of member variables for all tests. If a test fails, the contents of these member variables are used for additional output. Ensure that you catch any exceptions as shown in the example; otherwise, the unit test may terminate without providing useful output.

```c++
class CharRangeTest : public erbsland::UnitTest {
public:
    CharRange cr;
    CharRange cr2;

    auto additionalErrorMessages() noexcept -> std::string override {
        try {
            std::stringstream ss;
            ss << "Range 1: from 0x" << std::hex
                << std::setw(4) << std::setfill('0')
                << cr.from().value() << " to 0x"
                << std::setw(4) << std::setfill('0')
                << cr.to().value() << "\n";
            ss << "Range 2: from 0x" << std::hex
                << std::setw(4) << std::setfill('0')
                << cr2.from().value() << " to 0x"
                << std::setw(4) << std::setfill('0')
                << cr2.to().value() << "\n";
            return ss.str();
        } catch (...) {
            return "Unexpected exception when building char range diagnose text.";
        }
    }

    void testRange() {
        cr = CharRange();
        cr2 = CharRange();
        REQUIRE(cr.isEmpty());
        REQUIRE(cr.from().value() == 0xffffffu);
        REQUIRE(cr.to().value() == 0xffffffu);
        // ...
    }
};
```

By implementing the `additionalErrorMessages()` method, you can include custom error details in your test output, which can be useful for diagnosing issues when a test fails.

### Adding Context to Sub-Calls

For complex tests, you can split them into nested functions to improve organization and readability. To obtain more detailed information about the calling tree, wrap the sub-function call with the `WITH_CONTEXT(...)` macro.

```c++
class SaturationMathCastTest : public erbsland::UnitTest {
public:
    template<typename TargetValue, typename SourceValue>
    void checkUnchanged(SourceValue value) {
        auto result = saturatingCast<TargetValue, SourceValue>(value);
        REQUIRE(result == value);
        auto testResult = willSaturatingCastOverflow<TargetValue, SourceValue>(value);
        REQUIRE(testResult == false);
    }

    void testUnchangedCasts() {
        WITH_CONTEXT(checkUnchanged<uint8_t,  uint8_t >(0));
        WITH_CONTEXT(checkUnchanged<uint8_t,  uint16_t>(0));
        WITH_CONTEXT(checkUnchanged<uint8_t,  uint32_t>(0));
        WITH_CONTEXT(checkUnchanged<uint8_t,  uint64_t>(0));
        WITH_CONTEXT(checkUnchanged<uint16_t, uint8_t >(0));
        WITH_CONTEXT(checkUnchanged<uint16_t, uint16_t>(0));
        WITH_CONTEXT(checkUnchanged<uint16_t, uint32_t>(0));
        WITH_CONTEXT(checkUnchanged<uint16_t, uint64_t>(0));
    }
};
```

If a test within the `checkUnchanged` function fails, the output will also include the line number of the function call, making it easier to identify the source of the failure.

### Adding Meta Information to Test Functions and Classes

#### Add Tags with `TAGS(...)`

You can use the `TAGS(...)` macro to add any number of tags to test classes or functions. These tags can help you include or exclude specific groups of test classes or functions when running tests.

```c++
TAGS(HeavyCPU)
class ExampleTest : public el::UnitTest {
public:
    // 
    TAGS(LongRun ExtensiveTest)
    void testEveryCombination() {
        // ...
    }
};
```

The usage of tags is flexible and depends on your preferences.

#### Add Targets with `TESTED_TARGETS(...)`

The `TESTED_TARGETS(...)` macro allows you to add information about which targets a given test class or test function is testing. You can use these targets to include or exclude a group of test classes or functions from the test.

```c++
TESTED_TARGETS(Nanoseconds Microseconds Milliseconds Seconds Minutes Hours Days Weeks Amount)
class TimeAmountsTest : public el::UnitTest {
public:
    // ...
    TESTED_TARGETS(Nanoseconds)
    void testNanoseconds() {
        // ...
    }
};
```

You can specify any number of identifiers, separated by whitespace. Although the identifiers don't need to be related to the tested code, it's recommended to use the names of tested classes and/or functions. You are free to use identifiers in any way you like to group targets.

#### Skip Tests by Default with `SKIP_BY_DEFAULT()`

Using the `SKIP_BY_DEFAULT()` macro, you can mark test classes or functions that should not be executed by default. Classes and functions marked with this macro will only be executed if they are explicitly included using a command-line argument when starting the unit test.

```c++
SKIP_BY_DEFAULT()
class ExampleTest : public el::UnitTest {
public:
    // 
    SKIP_BY_DEFAULT()
    void testEveryCombination() {
        // ...
    }
};
```

#### Combine `TAGS(...)`, `TESTED_TARGETS(...)` and `SKIP_BY_DEFAULT()`

You can combine `TAGS`, `TESTED_TARGETS`, and `SKIP_BY_DEFAULT` for every class and test function by separating them with whitespace:

```c++
TAGS(MyTag)
TESTED_TARGETS(Example)
SKIP_BY_DEFAULT()
class ExampleTest : public el::UnitTest {
public:
    //
    TAGS(MyTag) TESTED_TARGETS(Example) SKIP_BY_DEFAULT()
    void testEveryCombination() {
        // ...
    }
};
```

This flexibility allows you to define meta information for each test class and function, making it easier to manage and filter tests based on your requirements.

### Writing Output to the Console

When investigating a problem, you may find it helpful to have additional output displayed on the console. Use the `consoleWriteLine()` method to write text to the console from within any test.

### Obtain the Path of the Unit Test Executable

When working with test data, it's useful to know the location of the test executable. The most reliable way to obtain this information is by calling the `unitTestExecutablePath()` method, which returns the absolute path to the unit test executable. You can use this as a base to locate test data that you have prepared in the binary directory of the test. This ensures that your tests can access the required data files without any issues, regardless of the working directory.

## Command Line Options for the Unit Test Executable

The unit test executable supports the following command-line arguments:

- `-h` / `--help`: Display help information.
- `-v` / `--verbose`: Display verbose messages, including skipped tests.
- `-e`: Stop at the first error.
- `-l` / `--list`: List all test suites and tests without running them.
- `-c` / `--no-color`: Disable colorized output and status updates.
- `-s` / `--no-summary`: Do not list the first three errors at the end of the test run.

- `name:<name>`: Run only tests with the specified test or class name (case-sensitive).
- `+name:<name>`: Include tests with the specified test or class name, even if they are optional.
- `-name:<name>`: Exclude tests with the specified test or class name.

- `target:<target>`: Run only tests for the specified target.
- `+target:<target>`: Include tests for the specified target, even if they are optional.
- `-target:<target>`: Exclude tests for the specified target.

- `tag:<tag>`: Run only tests with the specified tags.
- `+tag:<tag>`: Include tests with the specified tags, even if they are optional.
- `-tag:<tag>`: Exclude tests with the specified tags.

By default, all tests that are not marked with `SKIP_BY_DEFAULT()` are enabled. You can individually add (`+`) or remove (`-`) tests from this initial set using the options above.

When specifying one or more options like `<opt>:<tag>`, only tests with the given tags are enabled. Further `+/-` options will modify this set.

The processing order of the options is `<opt>`, `+<opt>`, `-<opt>` and does not depend on their order on the command line. Therefore, the `-` options always have the highest priority and will skip the specified tests regardless of any other options.

## Bugreports and Feature Requests

Please use the GitHub issue tracker to report bugs or request features:

https://github.com/erbsland-dev/erbsland-unittest/issues

## License

Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev/
Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch/

This program is free software: you can redistribute it and/or modify it under the terms of the
GNU Lesser General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program.
If not, see <https://www.gnu.org/licenses/>.

