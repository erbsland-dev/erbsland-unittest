
.. index::
    !single: Goals

Goals
=====

Dependency-Free
---------------

In complex projects, we have often found that unit test dependencies can create conflicts with the targets being tested. To avoid such issues, we have developed a C++ unit testing system that relies solely on a moderately modern C++ compiler and its standard library, with no additional libraries required.

For a more convenient user experience, the CMake build system and Python are required. These tools automatically generate the necessary code to register test classes, functions, and their metadata. While writing unit tests without CMake and Python is not our primary focus, it is theoretically possible to do all the registration manually.

User-Friendly
-------------

Writing unit tests is a crucial part of developing secure and reliable software, but it can be a tedious and time-consuming task. With this in mind, we designed our unit test system to minimize setup time and reduce the required files and code structures. A minimal working unit test only requires a `CMakeLists.txt`, `main.cpp`, and `ExampleTest.hpp` file. See the examples later in this document.

You can quickly expand your tests by adding tags, target information, and additional error messages. This allows you to easily scale from a minimal working test to a comprehensive test suite.

Minimalistic
------------

We believe that unit tests with an excessive number of macros and test functions do not necessarily result in better tests; instead, they can make the unit tests overly complex. Our unit test system adopts a minimalistic approach, providing a handful of test macros that cover most use cases while maintaining easy extensibility.

Modern
------

Our unit test system is written in modern C++ (up to C++17) but also works with the latest C++20 features if needed. It is compatible with a wide range of compilers, even those that may not yet fully support C++17 features.

Secure
------

Unit tests are often run automatically on build servers. Our unit test system is designed conservatively with security in mind, ensuring a minimal attack surface. The dependency-free architecture makes it easy to verify all execution paths, provided there are no serious flaws in the C++ standard library being used.

Scalable
--------

Sometimes one single test is needed, and sometimes your unit test has thousands of tests that have to be organized into a structure and annotated with metadata. Our unit test system is flexible enough to be useful from minimal up to large and complex testing projects.

