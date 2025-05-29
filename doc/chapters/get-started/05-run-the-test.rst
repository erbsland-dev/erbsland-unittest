.. _run-the-test:

Run the Test!
=============

Running your unit tests is the final step to validate your setup. Follow the instructions below to run and manage your tests effectively.

Run the Unit Test
-----------------

Navigate to the `unittest` directory in your build environment and execute the unit test binary:

.. code-block:: bash

    ./unittest/unittest

If you followed the earlier steps and created a test suite with a single test, the output should look like this:

.. code-block:: none

    ===[ Running 1 test suites with 1 tests ]===
    Start Time: Tue Apr 25 10:41:23 2025
    Filter: no filter set
    Suite: Basic OK!
      Test: Magic OK!
    Total Test Duration: 8.63e-05 seconds
    ===[ SUCCESS | Successfully run all tests without errors. ]===

For the **complete example**, the output includes more details, such as errors and additional tests:

.. code-block:: none

    ===[ Running 2 test suites with 4 tests ]===
    Start Time: Tue Apr 25 10:09:11 2025
    Filter: no filter set
    Suite: Basic OK!
      Test: Magic OK!
      Test: NameSetAndGet OK!
      Test: IsNamePalindrome FAILED!
    [1]: ~/erbsland-unittest-example/unittest/src/BasicTest.cpp:57: REQUIRE(exampleLib.isNamePalindrome())
    Suite: Context OK!
      Test: NameSetAndGet FAILED!
    [2]: ~/erbsland-unittest-example/unittest/src/ContextTest.cpp:52: REQUIRE_FALSE(exampleLib.getNameLength() == expectedSize)
    [1]: ~/erbsland-unittest-example/unittest/src/ContextTest.cpp:63: WITH_CONTEXT(setAndVerifyName_error("Lisa"))
    ExampleLib:
        getName() = "Lisa"
        getNameLength() = 4

    ===[ ERROR SUMMARY ]===
    Error 1 - Basic / IsNamePalindrome FAILED!
    [1]: ~/erbsland-unittest-example/unittest/src/BasicTest.cpp:57: REQUIRE(exampleLib.isNamePalindrome())
    Error 2 - Context / NameSetAndGet FAILED!
    [2]: ~/erbsland-unittest-example/unittest/src/ContextTest.cpp:52: REQUIRE_FALSE(exampleLib.getNameLength() == expectedSize)
    [1]: ~/erbsland-unittest-example/unittest/src/ContextTest.cpp:63: WITH_CONTEXT(setAndVerifyName_error("Lisa"))
    ===[ ERROR | 2 errors while running the tests. ]===

.. card:: Why the Errors?

    Don’t worry about the errors in the complete example. They are intentionally included to demonstrate the framework’s features, such as error handling and contextual output.

Get Command Line Help
---------------------

To explore all available command-line options for your test suite, use the ``-h`` or ``--help`` option:

.. code-block:: bash

    ./unittest/unittest --help

This outputs a list of options:

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

By default, all tests not marked with ``SKIP_BY_DEFAULT()`` are enabled. Use the ``+`` and ``-`` options to customize the test set.

List All Tests, Tags, and Targets
---------------------------------

To get an overview of all test suites, tags, and targets in your test suite, use the `-l` or `--list` option:

.. code-block:: bash

    ./unittest/unittest --list

For the **complete example**, the output looks like this:

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

- **Suite Names**: Displayed after "Suite".
- **Target Identifiers**: Shown in angle brackets (``<>``).
- **Tags**: Shown in square brackets (``[]``).

Filtering Tests
---------------

You can run specific tests using tags, names, or targets. For example, to run the ``IsNamePalindromeBruteForce`` test, use the tag ``long-test``:

.. code-block:: bash

    ./unittest/unittest tag:long-test

The output will look like this:

.. code-block:: none

    ===[ Running 1 test suites with 1 tests ]===
    Start Time: Tue Apr 25 10:48:11 2025
    Filter:
      Tags: only use 'long-test'
    Suite: Long OK!
    (...)
      Test: IsNamePalindromeBruteForce OK!
    Total Test Duration: 85.1 seconds
    ===[ SUCCESS | Successfully run all tests without errors. ]===

This test takes longer to execute, especially in debugging mode.

With these tools, you can efficiently run and manage your test suite. Let’s proceed to explore what’s next.

.. button-ref:: whats-next
    :color: success
    :align: center
    :expand:
    :class: sd-fs-5 sd-font-weight-bold sd-p-2

    What's Next? →

