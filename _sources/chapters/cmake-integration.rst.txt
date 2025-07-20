.. _cmake:
.. index::
    !single: cmake
    single: integration
    single: cmake integration

*****************
CMake Integration
*****************

*Erbsland Unit Test* offers a straightforward CMake integration, enabling you to create unit tests with minimal additional effort. We suggest using the following directory structure for your CMake project:

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
        │   ├── data                  # Test data
        │   │   └── (...)
        │   └── CMakeLists.txt        # Unit test CMake configuration
        └── CMakeLists.txt            # Root CMake configuration

Configuring the Root CMake File
===============================

The root CMake file consolidates the tested application or library, its dependencies, the *Erbsland Unit Test* framework, and the actual unittest into a single project.

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.23)
    project(ExampleUnitTest)
    add_subdirectory(erbsland-unittest)
    add_subdirectory(example-lib)
    add_subdirectory(unittest)
    enable_testing()
    add_test(
            NAME unittest
            COMMAND $<TARGET_FILE:unittest> --no-color
    )

In this root CMake file, simply include the configurations from the subdirectories of all necessary components. The order in which you add them is crucial. First, add the Erbsland Unit Test library, followed by the dependencies for the application or library you're testing, then your application or library itself, and finally, your unit test project.

At the end of the root CMake file, CTest integration is activated by defining a test with the name `unittest`. This test refers to the `unittest` executable defined in the unit test subproject. By enabling CTest this way, you can conveniently run your tests using the `ctest` command from the build directory, ensuring seamless integration into continuous integration pipelines and local development workflows.

Configuring the Unit Test Project
=================================

A typical unit test configuration for a small project appears as follows:

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.23)
    project(unittest)
    add_executable(unittest
            src/main.cpp
            src/BasicTest.hpp
            src/ContextTest.hpp
            src/LongTest.hpp)
    target_compile_features(unittest PRIVATE cxx_std_20)
    target_link_libraries(unittest PRIVATE erbsland-unittest-example-lib)
    target_include_directories(unittest PRIVATE ../example-lib/src)
    erbsland_unittest(TARGET unittest)

Start by adding a new executable to the project using ``add_executable``, specifying the target name ``unittest`` and all source files.

Then, include the necessary ``target_link_libraries`` and ``target_include_directories`` commands to access and link the unit test to the tested application. Depending on the complexity of the application or library being tested, this step may require more detailed configuration.

Finally, invoke ``erbsland_unittest`` with the ``TARGET`` parameter set to the name of your unit test executable, which in this example is ``unittest``.

The ``erbsland_unittest`` call performs several tasks for you: it first adds the required include directories and links your unit test with the Erbsland Unit Test library. It then integrates the metadata script into the project, automatically generating all necessary structures to register your test suites, test functions, and added metadata.

Utilizing the ``erbsland_unittest`` Function
============================================

The ``erbsland_unittest`` function streamlines your unit test project configuration by accomplishing various tasks. It adds necessary include directories, links the unit test with the Erbsland Unit Test library, and integrates the metadata script to automatically generate essential structures for registering test suites, test functions, and added metadata.

How to Use
----------

.. code-block:: cmake

    erbsland_unittest(
        TARGET <target_name>
        [PRECOMPILE_HEADERS]
        [NO_LINK_SETTINGS]
        [COPY_TEST_DATA <relative test data path>]
        [ENABLE_DATA_DEPS])

- ``TARGET``: (required) Sets the target name for your unit test executable.
- ``PRECOMPILE_HEADERS``: (optional) Activates precompiled headers for the unit test.
- ``NO_LINK_SETTINGS``: (optional) Deactivates automatic linking of the unit test, useful if you prefer manual linking or if the Erbsland Unit Test library is already part of another linked library.
- ``COPY_TEST_DATA``: (optional) Defines a path to test data, relative to the calling ``CMakeLists.txt`` file, which will be copied to the build directory. Use ``unitTestExecutablePath()`` in your unit test to locate the data when running unit tests from the build directory.
- ``ENABLE_DATA_DEPS``: (optional) When test data is copied, checks all test files for changes.

About Test Data Dependencies
----------------------------

Test data handling in the build process is controlled by the ``COPY_TEST_DATA`` and ``ENABLE_DATA_DEPS`` options of the ``erbsland_unittest`` function.

By default, when you provide ``COPY_TEST_DATA``, the test data is copied to the build directory as a post-build step. This means the files are only updated when the unit test binary is rebuilt. For most use cases—especially when the test suite is not under frequent development—this behavior is efficient and sufficient.

However, if your workflow requires that test data files reflect changes immediately without needing a rebuild of the unit test binary, you should enable the ``ENABLE_DATA_DEPS`` option. When this is set:

* A dedicated dependency target is created.
* The build system automatically tracks all test data files.
* Changes to any of these files will trigger an update, even if no source code has changed.

This mode ensures your test data is always current during development and continuous integration runs.

.. note::

    Enabling ``ENABLE_DATA_DEPS`` introduces additional overhead during dependency scanning. This is particularly noticeable when you have a large number of small test data files. If performance becomes an issue, consider disabling this option and triggering test data updates manually when needed.

Example
-------

The example below configures the ``unittest`` target and specifies that all test data from the ``data`` directory (refer to the example structure above) should be copied to the build directory.

.. code-block:: cmake

    erbsland_unittest(
            TARGET unittest
            COPY_TEST_DATA "data"
            ENABLE_DATA_DEPS)
