.. _advanced:
.. index::
    single: Advanced Usages

Advanced Usages
===============

Sharing Functionalities with a Common Base Class
------------------------------------------------

You can create a base class to provide shared functionality across your test classes. However, ensure that the name of the base class does **not** end with ``Test`` to avoid conflicts.

:file:`CommonBase.hpp`: Base Class for Shared Functions

.. code-block:: cpp

    #pragma once
    #include <erbsland/unittest/UnitTest.hpp>
    // ...
    class CommonBase : public el::UnitTest {
    public:
        void commonTestFunction() {
            // ...
        }
        // ...
    };

:file:`ExampleTest.hpp`: Subclass Using the Base Class

.. code-block:: cpp

    #pragma once
    #include "CommonBase.hpp"
    // ...
    class ExampleTest : public UNITTEST_SUBCLASS(CommonBase) {
    public:
        void testExample() {
            WITH_CONTEXT(commonTestFunction());
            // ...
        }
        // ...
    };

.. important::
    When using a shared base class for your unit tests, wrap the parent class name with the macro ``UNITTEST_SUBCLASS``. This ensures that the class is properly detected by the CMake automation system.

Organizing Your Tests with Labels
----------------------------------

For better organization, always label your test classes and functions using the ``TESTED_TARGETS`` and ``TAGS`` macros. These labels help identify the scope and nature of each test, simplifying filtering and debugging.

Adding Tags to Test Classes and Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can add tags using the ``TAGS`` macro to describe the type of test. Simply place the macro before the class or function definition:

.. code-block:: cpp

    TAGS(HeavyCPU)
    class ExampleTest : public el::UnitTest {
    public:
        //
        TAGS(LongRun ExtensiveTest)
        void testEveryCombination() {
            // ...
        }
    };

- **Multiple Tags**: Tags can be separated by spaces, allowing you to assign multiple labels to a single test or test suite.
- **No Code Impact**: The macros expand to nothing during compilation and do not influence your code execution. However, they are read by the CMake integration and incorporated into the test metadata.

Adding Targets to Track Tested Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Use the ``TESTED_TARGETS`` macro to note which functions or classes are being tested. This enables you to selectively run tests targeting specific functions, improving your workflow when debugging or modifying code.

.. code-block:: cpp

    TESTED_TARGETS(Nanoseconds Microseconds Milliseconds Seconds Minutes Hours Days Weeks Amount)
    class TimeAmountsTest : public el::UnitTest {
    public:
        // ...
        TESTED_TARGETS(Nanoseconds)
        void testNanoseconds() {
            // ...
        }
    };

- **Purpose of Targets**: Targets specify which functionality is tested by a class or function.
- **Selective Execution**: Later, you can run only the tests targeting a specific function using command-line options.

Example: Marking Long-Running Tests
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Tags can also be used to classify certain tests as long-running or resource-intensive. Combine ``TAGS`` with ``SKIP_BY_DEFAULT()`` to exclude such tests from default execution:

.. code-block:: cpp

    TAGS(LongRun)
    SKIP_BY_DEFAULT()
    void testLongComputation() {
        // ...
    }

This setup skips the test during regular runs but allows it to be explicitly enabled with a command-line option:

.. code-block:: bash

    ./unittest/unittest +tag:LongRun

This approach ensures efficient testing workflows by skipping unnecessary tests while retaining the flexibility to run them when needed.
