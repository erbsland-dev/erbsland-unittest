
.. _cmake:
.. index::
    !single: cmake
    single: integration
    single: cmake integration

CMake Integration
=================

*Erbsland UnitTest* offers a straightforward CMake integration, enabling you to create unit tests with minimal additional effort. We suggest using the following directory structure for your CMake project:

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
-------------------------------

The root CMake file consolidates the tested application or library, its dependencies, the *Erbsland UnitTest* library, and the actual unittest into a single project.

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.20)
    project(ExampleUnitTest)
    add_subdirectory(erbsland-unittest)
    add_subdirectory(example-lib)
    add_subdirectory(unittest)

In this root CMake file, simply include the configurations from the subdirectories of all necessary components. The order in which you add them is crucial. First, add the Erbsland UnitTest library, followed by the dependencies for the application or library you're testing, then your application or library itself, and finally, your unit test project.

Configuring the Unit Test Project
---------------------------------

A typical unit test configuration for a small project appears as follows:

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.20)
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

The ``erbsland_unittest`` call performs several tasks for you: it first adds the required include directories and links your unit test with the Erbsland UnitTest library. It then integrates the metadata script into the project, automatically generating all necessary structures to register your test suites, test functions, and added metadata.

Utilizing the ``erbsland_unittest`` Function
--------------------------------------------

The ``erbsland_unittest`` function streamlines your unit test project configuration by accomplishing various tasks. It adds necessary include directories, links the unit test with the Erbsland UnitTest library, and integrates the metadata script to automatically generate essential structures for registering test suites, test functions, and added metadata.

How to Use
^^^^^^^^^^

.. code-block:: cmake

    erbsland_unittest(
        TARGET <target_name>
        [PRECOMPILE_HEADERS]
        [NO_LINK_SETTINGS]
        [COPY_TEST_DATA <relative test data path>])

- ``TARGET``: (required) Sets the target name for your unit test executable.
- ``PRECOMPILE_HEADERS``: (optional) Activates precompiled headers for the unit test.
- ``NO_LINK_SETTINGS``: (optional) Deactivates automatic linking of the unit test, useful if you prefer manual linking or if the Erbsland UnitTest library is already part of another linked library.
- ``COPY_TEST_DATA``: (optional) Defines a path to test data, relative to the calling ``CMakeLists.txt`` file, which will be copied to the build directory. Use ``unitTestExecutablePath()`` in your unit test to locate the data when running unit tests from the build directory.

Example
^^^^^^^

The example below configures the ``unittest`` target and specifies that all test data from the ``data`` directory (refer to the example structure above) should be copied to the build directory.

.. code-block:: cmake

    erbsland_unittest(TARGET unittest COPY_TEST_DATA "data")
