.. _write-the-test:

Write the Test!
===============

Creating the ``main.cpp`` File
------------------------------

The ``main.cpp`` file serves as the entry point for your unit tests. Follow these steps to create and configure it:

1. Create the directory and file for your unit test executable:

   .. code-block:: bash

       mkdir src
       nano main.cpp

2. Add the following content to the file:

   .. code-block:: cpp

       #include <erbsland/unittest/UnitTest.hpp>
       ERBSLAND_UNITTEST_MAIN();

.. card:: Explanation

    - **Macro Simplification**: The ``ERBSLAND_UNITTEST_MAIN();`` macro streamlines the creation of the `main` function, automatically setting up your test executable.
    - **Build Environment Compatibility**: Including a ``main.cpp`` file ensures proper recognition of the project language by the build environment and prevents potential issues during compilation.

    This approach provides flexibility for defining test classes in header files without requiring additional implementation files.

Creating Your First Unit Test
-----------------------------

To define your unit tests, follow these conventions:


.. grid:: 1 1 1 1
    :gutter: 3

    .. grid-item-card::

        **File Naming**
        ^^^
        Each test suite should reside in a file ending with ``Test``, either in a single ``.hpp`` or ``.cpp`` file, or a combination of ``.hpp`` and ``.cpp`` files.

    .. grid-item-card::

        **Class Naming**
        ^^^
        The file must contain a class with the **same name** as the file.

    .. grid-item-card::

        **Test Method Naming**
        ^^^
        Test functions should be **public methods** in the class and start with a lowercase ``test``.

Example: Basic Test Suite
~~~~~~~~~~~~~~~~~~~~~~~~~

Let’s create a test suite named ``BasicTest``:

1. Create the file ``BasicTest.cpp``:

   .. code-block:: bash

       nano BasicTest.cpp

2. Add the following content to define the test suite:

   .. code-block:: cpp

       #pragma once

       #include <erbsland/unittest/UnitTest.hpp>
       #include <ExampleLib.hpp>

       using erbsland::ExampleLib;

       class BasicTest : public el::UnitTest {
       public:
           void testMagic() {
               auto exampleLib = ExampleLib{};
               REQUIRE(exampleLib.getMagicWord() == "Magic");
           }
       }

.. card:: Key Points

    - **Include Guards**: The ``#pragma once`` directive ensures the header file is included only once during compilation.
    - **Assertions**: Use the ``REQUIRE`` macro to assert that test conditions are met, such as verifying the return value of ``getMagicWord()``.

Adding the Test Suite to the CMake Configuration
------------------------------------------------

To compile and include the test suite, update your `CMakeLists.txt` file:

1. Navigate to the parent directory and open the `CMakeLists.txt` file:

   .. code-block:: bash

       cd ..
       nano CMakeLists.txt

2. Update the file with the new test suite:

   .. code-block:: cmake

       cmake_minimum_required(VERSION 3.23)
       project(unittest)
       add_executable(unittest
               src/BasicTest.cpp    # Add the new test suite
               src/main.cpp)
       target_compile_features(unittest PRIVATE cxx_std_20)
       target_link_libraries(unittest PRIVATE erbsland-unittest-example-lib)
       target_include_directories(unittest PRIVATE ../example-lib/src)
       erbsland_unittest(TARGET unittest)

Summary
~~~~~~~

- **Test Suite Integration**: The `BasicTest.cpp` file is added to the executable target.
- **Seamless Compilation**: The updated CMake configuration ensures the new test suite is compiled and linked correctly.

By completing these steps, you have successfully created and configured your first test suite using the Erbsland Unit Test framework. You're now ready to build and run your tests!

.. button-ref:: build-the-test
    :color: success
    :align: center
    :expand:
    :class: sd-fs-5 sd-font-weight-bold sd-p-2

    Build the Test! →

