
.. index::
    !single: Quick Start

Quick Start
===========

Full Unit Test Example
----------------------

If you prefer learning through examples, take a look at the following repository:

👉 `Erbsland Unittest Example <https://github.com/erbsland-dev/erbsland-unittest-example>`_

Command Line Examples
---------------------

Throughout the guide, you will find command line examples demonstrating how to use the ``git`` command to add submodules to the project, create directories with ``mkdir``, and create or edit files with the text editor ``nano``. These examples are provided for illustration purposes and are not meant to imply that you must work exclusively in the command line to set up your project. We have found that most software developers can easily understand and follow these examples.

Formatting in Code Examples
---------------------------

To keep this guide short and easy to read, most comments and empty lines were removed from the code listings. This does not mean you should do the same for real world projects. Please follow the usual coding guidelines when formatting the code for unit tests and build files.

Recommended Project Structure
-----------------------------

*Erbsland UnitTest* is intended to be utilized as a GIT submodule within a larger project. To ensure seamless integration and easy usage, we recommend the following project structure for a project called ``erbsland-unittest-example``:

.. code-block:: none

    erbsland-unittest-example
        ├── <libraries>         # All dependent libraries for the `Example` project (submodules)
        ├── erbsland-unittest   # The Erbsland UnitTest library added as a submodule
        ├── example-lib         # The "Example" project to be tested by the unit test (added as submodule as well)
        │   ├── src (...)
        │   └── CMakeLists.txt
        ├── unittest            # The actual unit test, containing one or more unit test binaries
        │   ├── src             # The source files for the unit tests
        │   │   ├── main.cpp           # The main file for the executable
        │   │   ├── BasicTest.hpp      # Each set of tests has to be in a file ending in ...Test.hpp
        │   │   ├── ContextTest.hpp    # Each set contains a class with the same name of the file.
        │   │   └── LongTest.hpp
        │   ├── data            # Data for unit tests
        │   └── CMakeLists.txt
        └── CMakeLists.txt      # The root project CMake file

By following this structure, you can ensure that your project is well-organized and easy to navigate. It also makes it simple to manage your unit tests, ensuring efficient and thorough testing of your libraries and applications.

Create the Project Structure
----------------------------

.. code-block:: bash

    cd ~
    mkdir erbsland-unittest-example
    cd erbsland-unittest-example
    git init
    git submodule add https://github.com/erbsland-dev/erbsland-unittest.git erbsland-unittest
    git submodule init erbsland-unittest

Next, import the application or library and all dependencies as submodules to this project:

.. code-block:: bash

    git submodule add https://github.com/erbsland-dev/erbsland-unittest-example-lib.git example-lib
    git submodule init example-lib

Configuring the Root Project CMake File
---------------------------------------

To set up the root project CMake file, use the following template:

.. code-block:: bash

    nano CMakeLists.txt

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.20)
    project(ExampleUnitTest)
    add_subdirectory(erbsland-unittest)
    add_subdirectory(example-lib)
    add_subdirectory(unittest)

This configuration ensures that the project properly includes all necessary subdirectories for building and testing the ``erbsland-unittest-example`` project.

Setting Up the Unit Test CMake File
-----------------------------------

For a simple unit test with one executable, as illustrated in the example project structure, you'll need a CMake file like the one shown below:

.. code-block:: bash

    mkdir unittest
    cd unitest
    nano CMakeLists.txt

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.20)
    project(unittest)
    add_executable(unittest
            src/main.cpp)
    target_compile_features(unittest PRIVATE cxx_std_17)
    target_link_libraries(unittest PRIVATE erbsland-unittest-example-lib)
    target_include_directories(unittest PRIVATE ../example-lib/src)
    erbsland_unittest(TARGET unittest)

This configuration sets up the ``unittest`` project with C++17 as the standard. It also adds the ``src`` subdirectory and incorporates the *Erbsland UnitTest* system.

The last function call ``erbsland_unittest(TARGET unittest)`` does most of the work for you. It configures your executable to link the unit test library and adds the correct include path. Also, it creates an additional target ``unittestMetaGen`` that generates all the required additional code, so you don't have to do it.

Writing the Tests
-----------------

Creating the ``main.cpp`` File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``main.cpp`` file serves as the entry point for your unit tests. To create and set it up, follow the example below:

.. code-block:: bash

    mkdir src
    nano main.cpp

.. code-block:: cpp

    #include <erbsland/unittest/UnitTest.hpp>
    ERBSLAND_UNITTEST_MAIN();

By using the ``ERBSLAND_UNITTEST_MAIN();`` macro, you can efficiently create the main function for your unit test executable.

The ``main.cpp`` file is purposefully added manually to the project. Automatically generating it can cause issues with build environments, as they often determine the compiler language based on the project files. Our unit testing system allows test classes to be declared in header files without the need for additional implementation files. Having the `main.cpp` file in place helps prevent many potential problems.

Creating Your First Unit Test
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To create unit tests using this system, follow these essential guidelines:

- **Each set of tests must be in a ``.hpp`` file or a combination of ``.hpp`` and ``.cpp`` files with a name ending in ``Test``.**
- **The file must contain a ``class`` that shares the _same_ name as the file.**
- **Tests should be _public_ methods within that class, with names beginning with lowercase ``test``.**

Now, let's create your first unit test. In most cases, using a single header file for all tests is more efficient, as the unit test system will automatically generate any missing implementation.

Begin by creating a test suite called ``Basic``. Name the file ``BasicTest.hpp`` and assign the same name ``BasicTest`` to the class inside the file:

.. code-block:: bash

    nano BasicTest.hpp

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

Next add this file to the ``CMakeLists.txt`` file:

.. code-block:: bash

    cd ..
    nano CMakeLists.txt

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.20)
    project(unittest)
    add_executable(unittest
            src/BasicTest.hpp    # <--- added the file
            src/main.cpp)
    target_compile_features(unittest PRIVATE cxx_std_17)
    target_link_libraries(unittest PRIVATE erbsland-unittest-example-lib)
    target_include_directories(unittest PRIVATE ../example-lib/src)
    erbsland_unittest(TARGET unittest)

Create the Build Environment and Compile the Test
-------------------------------------------------

In order to compile the test, you need to create the build files using CMake first:

.. code-block:: bash

    cd ~
    mkdir cmake-build
    cd cmake-build
    cmake ~/erbsland-unittest-example

If everything is ok, you will get an output like this:

.. code-block:: none

    -- The C compiler identification is (...)
    -- The CXX compiler identification is (...)
    -- Detecting C compiler ABI info
    (...)
    -- Found Python3: (...) (found suitable version "3.11.2", minimum required is "3.11") found components: Interpreter
    -- Configuring done
    -- Generating done
    -- Build files have been written to: ~/erbsland-unittest-example

Next, compile the unit test:

.. code-block:: bash

    make

.. code-block:: none

    (...)
    [100%] Linking CXX executable unittest
    [100%] Built target unittest

Run the Test
------------

Now you can run the test. It is placed in the subdirectory `unittest` of your build environment:

.. code-block:: bash

    ./unittest/unittest

If you followed along and just created a unit test with a single test function, you will get this output:

.. code-block:: none

    ===[ Running 1 test suites with 1 tests ]===
    Start Time: Tue Apr 25 10:41:23 2023
    Filter: no filter set
    Suite: Basic OK!
      Test: Magic OK!
    Total Test Duration: 8.63e-05 seconds
    ===[ SUCCESS | Successfully run all tests without errors. ]===

For the *complete example* you will get the following output, in colour if your console is supporting them:

.. code-block:: none

    ===[ Running 2 test suites with 4 tests ]===
    Start Time: Tue Apr 25 10:09:11 2023
    Filter: no filter set
    Suite: Basic OK!
      Test: Magic OK!
      Test: NameSetAndGet OK!
      Test: IsNamePalindrome FAILED!
    [1]: ~/erbsland-unittest-example/unittest/src/BasicTest.hpp:57: REQUIRE(exampleLib.isNamePalindrome())
    Suite: Context OK!
      Test: NameSetAndGet FAILED!
    [2]: ~/erbsland-unittest-example/unittest/src/ContextTest.hpp:52: REQUIRE_FALSE(exampleLib.getNameLength() == expectedSize)
    [1]: ~/erbsland-unittest-example/unittest/src/ContextTest.hpp:63: WITH_CONTEXT(setAndVerifyName_error("Lisa"))
    ExampleLib:
        getName() = "Lisa"
        getNameLength() = 4

    ===[ ERROR SUMMARY ]===
    Error 1 - Basic / IsNamePalindrome FAILED!
    [1]: ~/erbsland-unittest-example/unittest/src/BasicTest.hpp:57: REQUIRE(exampleLib.isNamePalindrome())
    Error 2 - Context / NameSetAndGet FAILED!
    [2]: ~/erbsland-unittest-example/unittest/src/ContextTest.hpp:52: REQUIRE_FALSE(exampleLib.getNameLength() == expectedSize)
    [1]: ~/erbsland-unittest-example/unittest/src/ContextTest.hpp:63: WITH_CONTEXT(setAndVerifyName_error("Lisa"))
    ===[ ERROR | 2 errors while running the tests. ]===

No worries about all the errors, the example library and unit test produce these intentionally do demonstrate the various features.

Get Command Line Help
^^^^^^^^^^^^^^^^^^^^^

To get a list with all command line options available for the test suite, use the ``-h`` or ``--help`` option:

.. code-block:: bash

    ./unittest/unittest --help

.. code-block:: none

    Erbsland Unit Test Help:
      -h/--help ......... Display this help
      -v/--verbose ...... Display verbose messages. Skipped tests.
      -e ................ Stop at the first error.
      -l/--list ......... List all suites and tests. Do not run any test.
      -c/--no-color ..... Do not colorize the output and disable status updates.
      -s/--no-summary ... Do not list the first three errors at the end of the run.
      name:<name> ....... Exclusively run tests with the specified test or class name (case sensitive).
      +name:<name> ...... Run tests with the specified test or class name, even optional ones.
      -name:<name> ...... Skip tests with the specified test or class name.
      target:<target> ... Exclusively run tests for the specified target.
      +target:<target> .. Run tests with for the specified target, even optional ones.
    -target:<target> .. Skip tests with for the specified target.
    tag:<tag> ......... Exclusively run tests with the specified tags.
    +tag:<tag> ........ Run tests with the specified tags, even optional ones.
    -tag:<tag> ........ Skip tests with the specified tags.

By default all tests that are not marked with ``SKIP_BY_DEFAULT()`` are enabled.
You can individually add ``+`` or remove ``-`` tests from this initial set.

If you specify one or more options like ``<opt>:<tag>``, only tests with
are enabled, and further ``+/-`` options will change this set.

The processing order of the options is ``<opt>``, ``+<opt>``, ``-<opt>`` and does not depend
on the order how they are specified on the command line. Therefore ``-`` always have
the highest priority and will skip these tests no mather what was specified otherwise.

List all Tests, Tags and Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To get an overview of all tests, tags and targets that are compiled in a test suite, use the `-l` or `--list` option.

.. code-block:: bash

    ./unittest/unittest --list

The *complete example* produces this result:

.. code-block:: none

    ===[ List all test suites and tests ]===
    Suite: Basic <ExampleLib>
      Test: Magic <getMagicWord>
      Test: NameSetAndGet <getName, getNameLength, setName>
      Test: IsNamePalindrome <isNamePalindrome, setName>
    Suite: Context <ExampleLib>
      Test: NameSetAndGet <getName, getNameLength, setName>
    Suite: Long <ExampleLib>
      Test: (IsNamePalindromeBruteForce) [long-test] <isNamePalindrome, setName>
    Done!

The names directly after the colon are the **names** of the test suites and functions. The identifiers in the angle brackets (``<>``) are **targets** and the identifiers in the square brackets (``[]``) are **tags**. If the name of a suite or test is in round brackets (``()``) it means that it is skipped by default.

Filtering Tests, Select the Tests you Like to Run
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you look at this list, you see that the test ``IsNamePalindromeBruteForce`` is skipped by default and the tag ``long-test`` indicates why. So, lets start only the test with the tag ``long-test``:

.. code-block:: bash

    ./unittest/unittest tag:long-test

.. code-block:: none

    ===[ Running 1 test suites with 1 tests ]===
    Start Time: Tue Apr 25 10:48:11 2023
    Filter:
      Tags: only use 'long-test'
    Suite: Long OK!
    Testing name 0: baaaaa
    Testing name 1000000: phxeca
    Testing name 2000000: dpujea
    Testing name 3000000: rwroga
    (...)
    Testing name 293000000: vylery
    Testing name 294000000: jgjjty
    Testing name 295000000: xngovy
    Testing name 296000000: lvdtxy
    Testing name 297000000: zcbyzy
      Test: IsNamePalindromeBruteForce OK!
    Total Test Duration: 85.1 seconds
    ===[ SUCCESS | Successfully run all tests without errors. ]===

This is indeed a long-running test (only when compiled for debugging).

What's Next?
------------

-   If you not already did, have a look at the full `example <https://github.com/erbsland-dev/erbsland-unittest-example>`_ that demonstrates most of the basic features of the unit test.

    👉 `UnitTest Example <https://github.com/erbsland-dev/erbsland-unittest-example>`_ 👈

-   For more details about the unit testing system, have a look into the `documentation <https://github.com/erbsland-dev/erbsland-unittest/wiki/Documentation>`_:

    📚 `Documentation <https://github.com/erbsland-dev/erbsland-unittest/wiki/Documentation>`_ 📚

