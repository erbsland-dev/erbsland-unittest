
.. _structure-and-naming:
.. index::
    !single: Structure
    !single: Naming

Structure and Naming
====================

In this chapter, we explain how your unittest project should be structured and how you shall name your classes and test methods.

Structure
---------

We recommend the following project structure for small unit tests with 1-50 test suites:

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
        │   │   ├── <Suite1>Test.hpp
        │   │   ├── <Suite2>Test.hpp
        │   │   └── (...)
        │   ├── data
        │   └── CMakeLists.txt
        └── CMakeLists.txt

We recommend adding the *Erbsland UnitTest*, all required libraries and the tested application or library as submodule. This simplifies versioning and updates and let you work more efficiently on the tests.

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
        │   │   │   ├── <Suite1>Test.hpp
        │   │   │   ├── <Suite2>Test.hpp
        │   │   │   └── (...)
        │   │   ├── <module2>
        │   │   │   ├── CMakeLists.txt
        │   │   │   ├── <Suite1>Test.hpp
        │   │   │   ├── <Suite2>Test.hpp
        │   │   │   └── (...)
        │   │   ├── <module3>
        │   │   │   ├── CMakeLists.txt
        │   │   │   ├── <Suite1>Test.hpp
        │   │   │   ├── <Suite2>Test.hpp
        │   │   │   └── (...)
        │   │   └── (...)
        │   └── CMakeLists.txt
        └── CMakeLists.txt

Configure this project that several executables are compiled, one for each module. To run all unittests you can either write a shell script or utilize the CMake build system to compile and run all executables for you.

Organizing your Tests
---------------------

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
-----------------------------

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
    :caption: The file ``ExampleTest.hpp``

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
------------------------------------

We recommend to declare and implement the whole test class in either the header or implementation file, but you can split the declaration and implementation into separate ``.hpp`` and ``.cpp`` files if this makes more sense for you.

Helper Functions
----------------

Your test class can contain any number of additional functions to perform tests, yet they must not start with lowercase ``test``.

Instance Variables
------------------

You can use instance variables that are shared between all tests of your test suite. This is in many cases even beneficial, as it allows easier debugging of the states in case on an error.

