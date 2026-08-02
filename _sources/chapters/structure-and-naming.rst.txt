
.. _structure-and-naming:
.. index::
    !single: Structure
    !single: Naming

********************
Structure and Naming
********************

In this chapter, we explain how your unittest project should be structured and how you shall name your classes and test methods.

Structure for Separate Tests
============================

If you like to keep your tests in a separate repository, we recommend the following project structure for small unit tests with 1-50 test suites:

.. code-block:: none

    unittest-project
        ├── <required libraries>
        ├── <tested app/lib>
        │   ├── src (...)
        │   └── CMakeLists.txt
        ├── erbsland-unittest
        ├── unittest
        │   ├── src
        │   │   ├── main.cpp
        │   │   ├── <Suite1>Test.cpp
        │   │   ├── <Suite2>Test.cpp
        │   │   └── (...)
        │   ├── data
        │   └── CMakeLists.txt
        └── CMakeLists.txt

We recommend adding the *Erbsland Unit Test*, all required libraries and the tested application or library as submodule. This simplifies versioning and updates and let you work more efficiently on the tests.

So, even with rapid development on the tested application, you can update to newer tested revisions at your chosen time. Also, you have a way to go back to old revisions of the unittest and easy find out at which point a problem was introduced in the application (or test).

For large projects, with more than 50 test suites, we recommend the following project structure:

.. code-block:: none

    unittest-project
        ├── <required libraries>
        ├── <tested app/lib>
        │   ├── src (...)
        │   └── CMakeLists.txt
        ├── erbsland-unittest
        ├── unittest
        │   ├── src
        │   │   ├── main.cpp
        │   │   ├── <module1>
        │   │   │   ├── CMakeLists.txt
        │   │   │   ├── <Suite1>Test.cpp
        │   │   │   ├── <Suite2>Test.cpp
        │   │   │   └── (...)
        │   │   ├── <module2>
        │   │   │   ├── CMakeLists.txt
        │   │   │   ├── <Suite1>Test.cpp
        │   │   │   ├── <Suite2>Test.cpp
        │   │   │   └── (...)
        │   │   ├── <module3>
        │   │   │   ├── CMakeLists.txt
        │   │   │   ├── <Suite1>Test.cpp
        │   │   │   ├── <Suite2>Test.cpp
        │   │   │   └── (...)
        │   │   └── (...)
        │   └── CMakeLists.txt
        └── CMakeLists.txt

Configure this project that several executables are compiled, one for each module. To run all unittests you can either write a shell script or utilize the CMake build system to compile and run all executables for you.

Structure for Integrated Tests
==============================

If you prefer to integrate your unit tests directly into the repository of the tested project, we recommend the following structure:

.. code-block:: none

    tested-project
        ├── <required libraries>
        ├── include
        ├── src
        │   ├── ...
        │   └── CMakeLists.txt
        ├── test
        │   ├── erbsland-unittest
        │   ├── unittest
        │   │   ├── src
        │   │   │   ├── main.cpp
        │   │   │   ├── <Suite1>Test.cpp
        │   │   │   ├── <Suite2>Test.cpp
        │   │   │   └── (...)
        │   │   ├── data
        │   │   └── CMakeLists.txt
        │   └── CMakeLists.txt
        └── CMakeLists.txt

All test-related code is placed within the ``test`` subdirectory. This structure allows you to easily exclude tests when integrating the project as a library into another system or when packaging it for production.

We suggest adding the *Erbsland Unit Test* framework as a Git submodule:

.. code-block:: shell

    mkdir test
    git submodule add https://github.com/erbsland-dev/erbsland-unittest.git test/erbsland-unittest
    git submodule init test/erbsland-unittest

This setup ensures that your testing framework is version-controlled alongside your codebase, making it easy to maintain consistency and reproduce test results across different environments.

Optional Test Inclusion via CMake
---------------------------------

Because tests are neatly isolated in the ``test`` directory, you can make test compilation optional by extending your top-level ``CMakeLists.txt`` file:

.. code-block:: cmake
    :emphasize-lines: 3, 7-15

    cmake_minimum_required(VERSION 3.23)
    project(ExampleProject)
    option(EXAMPLE_PROJECT_ENABLE_TESTS "Enable unit tests" OFF)

    # ... rest of your CMake configuration ...

    if(EXAMPLE_PROJECT_ENABLE_TESTS)
        # Add the unit tests
        add_subdirectory(test)
        enable_testing()
        add_test(
            NAME unittest
            COMMAND $<TARGET_FILE:unittest> --no-color
        )
    endif()

To enable testing during your build configuration, pass the option flag:

.. code-block:: shell

    cmake -S . -B cmake-build-debug -G Ninja -DEXAMPLE_PROJECT_ENABLE_TESTS=ON

This approach allows your test infrastructure to stay part of the main project while remaining completely optional and unobtrusive in environments where tests are not required.

Organizing your Tests
=====================

We designed this unit testing system to allow to organize the tests in modules, suites and blocks:

.. code-block:: none

    Project
        └── Test Module         => executable DateAndTimeTests.txt
            └── Test Suite      => class DateTest : public el::UnitTest { ... };
                └── Test Block  => void testConstruction() { ... }
                    └── Tests   => REQUIRE(dateTime.isEmpty())

For small projects we recommend this organisation of the tests:

- Create one single module (application) for all tests.
- Create at least one test suite for each tested class in your target.
- Create at least one test block for each tested function in your class.

For large projects we recommend the following organisation of the tests:

- Create one single module (application) for each logical part of your project.
- Create at least one test suite for each tested class in your target.
- Create at least one test block for each tested function in your class.

Naming of Classes and Methods
=============================

The automatic metadata generator will automatically register all your test classes and methods for you, and also extract your tags and markings. In order for the automatic registration system to work, you need to name your files, classes and methods according to the following rules:

- | The source files for test suites have to end in ``Test``.
  | Examples: ``ExampleTest.hpp``, ``ExampleTest.cpp``
- | The source files must use the suffixes ``.hpp`` and ``.cpp``.
  | Examples: ``ExampleTest.hpp``, ``ExampleTest.cpp``
- | Each source file of a test suite must contain the declaration and/or implementation of a class with the same name.
  | Example: The file ``ExampleTest.hpp`` contains the declaration ``class ExampleTest : (...)``.
- | Test classes must inherit :cpp:expr:`erbsland::unittest::UnitTest` or short :cpp:expr:`el::UnitTest`.
  | Example: ``class ExampleTest : public el::UnitTest { (...) };``
- Test functions in the test classes must be public ::cpp:expr:`void` functions with no arguments.
- The name of test functions must start with lowercase ``test``.

.. code-block:: cpp
    :caption: The file ``ExampleTest.cpp``

    #pragma once
    #include <erbsland/unittest/UnitTest.hpp>
    // ...
    class ExampleTest : public el::UnitTest {
    public:
        void testExample1() {
            // ...
        }

        void testExample2() {
            // ...
        }

        // ...
    };

Split Declaration and Implementation
====================================

You have the flexibility to organize your test classes in the way that best fits your project. While we recommend keeping the declaration and implementation of your test class together in a single file—either a header or source file—you may split them into separate ``.hpp`` and ``.cpp`` files if it better suits your structure or coding standards.

The *Erbsland Unit Test* framework fully supports all of the following patterns:

* Implementing the entire test class directly in a ``.cpp`` file without a separate header (our recommend solution).
* Declaring and implementing the test class entirely in a header file.
* Declaring the class in a ``.hpp`` file and implementing it in a corresponding ``.cpp`` file.

Choose the approach that keeps your test code clean, maintainable, and in harmony with the conventions of the rest of your project.

Helper Functions
================

Your test class can include any number of additional helper functions to improve readability and reduce duplication across your test cases. However, it's important that these functions **do not start with the lowercase prefix ``test``**, as this would cause them to be mistakenly registered as actual test methods.

If your helper function performs assertions or validation steps, we recommend wrapping its call in the ``WITH_CONTEXT`` macro. This macro captures the context from which the function is invoked and includes it in any resulting error messages, greatly improving diagnostics.

.. code-block:: cpp

    void setAndVerifyName(const std::string &name) {
        exampleLib.setName(name);
        auto expectedSize = name.size();
        REQUIRE(exampleLib.getName() == name);
        REQUIRE(exampleLib.getNameLength() == expectedSize);
    }

    TESTED_TARGETS(getName getNameLength setName)
    void testNameSetAndGet() {
        WITH_CONTEXT(setAndVerifyName({}));
        WITH_CONTEXT(setAndVerifyName("joe"));
        WITH_CONTEXT(setAndVerifyName("anna"));
        // ...
    }

Using ``WITH_CONTEXT`` ensures that if a failure occurs within a helper function, the name of the test and the specific call site are clearly visible in the output. This helps you pinpoint failing inputs without digging through stack traces.

For more details, refer to:

* :doc:`macros-for-tests` – to understand the purpose and scope of test macros like ``REQUIRE`` and ``WITH_CONTEXT``.
* :doc:`cmake-integration` – to see the framework detects test suites and methods by name.

Instance Variables
==================

You can declare instance variables within your test class to share state between test blocks. This is particularly useful when testing value types or modules whose internal state needs to be inspected after assertions. Shared instance variables make debugging easier and help reduce duplication in your test setup.

.. code-block:: cpp

    class ExampleTest final : public el::UnitTest {
    public:
        Example example;

        auto additionalErrorMessages() -> std::string override {
            try {
                return std::format(
                    "example.a = {}\nexample.b = {}\nexample.c = {}\n",
                    example.a, example.b, example.c
                );
            } catch (...) {
                return "Unexpected exception while collecting diagnostics.";
            }
        }

        void setUp() override {
            example = {};
        }

        void testConstruction() {
            example = Example{10};
            REQUIRE_EQUAL(example.a, 100);
            REQUIRE_EQUAL(example.b, 400);
            REQUIRE_EQUAL(example.c, 1000);

            example = Example{0};
            REQUIRE_EQUAL(example.a, 0);
            REQUIRE_EQUAL(example.b, 0);
            REQUIRE_EQUAL(example.c, 0);
        }

        // Additional test blocks can reuse `example` here
    };


