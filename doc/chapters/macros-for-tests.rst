
.. index::
    !single: Macros
    single: Test Macros

.. cpp:namespace:: erbsland::unittest::UnitTest

.. _macros:

Macros for Unittests
====================

*Erbsland UnitTest* has a small number of macros to perform the tests and for adding meta information to the test classes and methods.

Quick Overview
--------------

Macros for Tests
^^^^^^^^^^^^^^^^

- :c:expr:`REQUIRE(expression)`: Tests if the given expression is true.
- :c:expr:`REQUIRE_FALSE(expression)`: Tests if the given expression is false.
- :c:expr:`REQUIRE_THROWS(expression)`: Tests if the given expression throws an exception. Fails if it does not.
- :c:expr:`REQUIRE_THROWS_AS(exception class, expression)`: Tests if the given expression throws an exception of the specified type or derived type ``exception class>``. Fails if it does not.
- :c:expr:`REQUIRE_NOTHROW(expression)`: Tests if the given expression does *not* throw an exception. Fails if it does.

For all ``REQUIRE_...`` macros listed above, there is a corresponding ``CHECK_...`` version. These check versions only display a message in the output, but the test will not fail.

- :c:expr:`CHECK(expression)`: Like ``REQUIRE``, but only warns.
- :c:expr:`CHECK_FALSE(expression)`: Like ``REQUIRE_FALSE``, but only warns.
- :c:expr:`CHECK_THROWS(expression)`: Like ``REQUIRE_THROWS``, but only warns.
- :c:expr:`CHECK_THROWS_AS(exception class, expression)`: Like ``REQUIRE_THROWS_AS``, but only warns.
- :c:expr:`CHECK_NOTHROW(expression)`: Like ``REQUIRE_NOTHROW``, but only warns.

- :c:expr:`WITH_CONTEXT(expression)`: Executes the expression, but adds a context for error reporting.

Macros for Meta Data
^^^^^^^^^^^^^^^^^^^^

- :c:expr:`TAGS(tags)`: Adds a tag to a class or test function.
- :c:expr:`TESTED_TARGETS(targets)`: Adds a tested target to a class or test function.
- :c:expr:`SKIP_BY_DEFAULT()`: Skips a test or class by default.

Helper Macros
^^^^^^^^^^^^^

- :c:expr:`SOURCE_LOCATION()`: Gets the source location, used for the :cpp:expr:`runWithContext()` call.
- :c:expr:`ERBSLAND_UNITTEST_MAIN()`: Creates a simple main function to start the unittest.
- :c:expr:`UNITTEST_SUBCLASS()`: Marks a subclass that is derived from :cpp:expr:`UnitTest` in order that the CMake system will properly register the unittest class.


The :c:expr:`REQUIRE(expression)` Macro
---------------------------------------

The :c:expr:`REQUIRE` macro evaluates whether an expression is ``true``. If the expression results in ``false`` or throws an exception, the test suite stops with an error. The error message displays the exact location in the code where the error occurred and the tested expression.

.. code-block:: cpp

    void testNameSetAndGet() {
        std::string name = "Peter";
        exampleLib.setName(name);
        REQUIRE(exampleLib.getName() == name);
    }

You might be familiar with unit testing systems that use test macros like ``COMPARE(A, B)``, which return the actual compared values in case of an error. We found these to be less useful, as complex tests often involve values that cannot be easily converted into text.

To view the tested values, we recommend one of the following approaches.

For local and specialized tests, enclose the tested block with a :cpp:expr:`runWithContext()` call. The second lambda function displays all relevant information about the tested values in case of an error.

.. code-block:: cpp

    void testExample() {
        int x = 5;
        runWithContext(SOURCE_LOCATION(), [&]() {
            x = 9;
            REQUIRE(x == 10);
        }, [&]() {
            std::stringstream text;
            text << "x = " << x;
            return text.str();
        });
    }

We suggest using small test suites and instance variables shared between all test blocks. Then implement the :cpp:expr:`additionalErrorMessages()` to display the state of these variables if an error occurs. This approach not only makes tests easier to read, it also helps debugging them.

.. code-block:: cpp

    class ExampleTest : public el::UnitTest {
    public:
        int inputA{};
        std::string inputB{};
        bool expected{};

        auto additionalErrorMessages() -> std::string override {
            try {
                auto text = std::ostringstream{};
                text << "inputA = " << inputA << " / inputB = " << inputB << " / expected = " << expected << "\n";
                return text.str();
            } catch(...) {
                return {"Unexpected Exception"};
            }
        }

        void testExample() {
            Foo foo;
            inputA = 5;
            inputB = "example";
            expected = false;
            REQUIRE(foo.call(inputA, inputB) == expected)
            // ...
        }
        // ...
    };

The :c:expr:`REQUIRE_FALSE(expression)` Macro
---------------------------------------------

This test macro works like :c:expr:`REQUIRE`, but expects ``false`` as result. If the expression results in ``true`` or throws an exception, the test suite is stopped with an error.

The macro exists to have a more visual indicator a negative result is expected.

.. code-block:: cpp

    void testNameSetAndGet() {
        std::string name = "Peter";
        exampleLib.setName(name);
        REQUIRE_FALSE(exampleLib.isNamePalindrome());
    }

The :c:expr:`REQUIRE_THROWS(expression)` Macro
----------------------------------------------

This test macro expects that the given expression throws an exception. Any thrown exception is accepted. If the expression throws no exception, the test suite stops with an error.

.. code-block:: cpp

    void testNameSetAndGet() {
        auto exampleLib = ExampleLib{};
        exampleLib.setName("joe");
        REQUIRE_THROWS(exampleLib.isEvenPalindrome());
    }

The :c:expr:`REQUIRE_THROWS_AS(exception class, expression)` Macro
------------------------------------------------------------------

This macro works like :c:expr:`REQUIRE_THROWS` but you can also specify the class of the exception you expect.

.. code-block:: cpp

    void testNameSetAndGet() {
        auto exampleLib = ExampleLib{};
        exampleLib.setName("joe");
        REQUIRE_THROWS_AS(std::domain_error, exampleLib.isEvenPalindrome());
    }

The :c:expr:`REQUIRE_NOTHROW(expression)` Macro
-----------------------------------------------

This macro expects the expression throws no exception. Compared with :c:expr:`REQUIRE`, it does not expect and discards any return value of the expression.

The ``CHECK_...`` Macros
------------------------

The ``CHECK_...`` macros are counterparts to the ``REQUIRE_...`` macros. If the test in one of these macros fails, this is only reported as warning in the output, but does not stop the test suite.

The :c:expr:`WITH_CONTEXT(expression)` Macro
--------------------------------------------

This macro adds a context around the expression. In case a test fails in the nested expression, the line and file of the :c:expr:`WITH_CONTEXT` statement is also reported in the output.

Use this macro if you call additional test methods to get the original location of the call.

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

In case of a problem, you see the nested calls in the output:

.. code-block:: none

    Test: NameSetAndGet FAILED!
    [2]: /erbsland-unittest-example/unittest/src/ContextTest.hpp:56: REQUIRE_FALSE(exampleLib.getNameLength() == expectedSize)
    [1]: /erbsland-unittest-example/unittest/src/ContextTest.hpp:67: WITH_CONTEXT(setAndVerifyName("Lisa"))

You can nest as many :c:expr:`WITH_CONTEXT` macros as you like.

Add Tags with :c:expr:`TAGS(...)`
---------------------------------

You can use the :c:expr:`TAGS(...)` macro to add any number of tags to test classes or functions. These tags can help you include or exclude specific groups of test classes or functions when running tests.

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

The usage of tags is flexible and depends on your preferences.

Add Targets with :c:expr:`TESTED_TARGETS(...)`
----------------------------------------------

The c:expr:`TESTED_TARGETS(...)` macro allows you to add information about which targets a given test class or test function is testing. You can use these targets to include or exclude a group of test classes or functions from the test.

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

You can specify any number of identifiers, separated by whitespace. Although the identifiers don't need to be related to the tested code, we recommended to use the names of tested classes and/or functions.

Skip Tests by Default with :c:expr:`SKIP_BY_DEFAULT()`
------------------------------------------------------

Using the :c:expr:`SKIP_BY_DEFAULT()` macro, you can mark test classes or functions that should not be executed by default. Classes and functions marked with this macro will only be executed if they are explicitly included using a command-line argument when starting the unit test.

.. code-block:: cpp

    SKIP_BY_DEFAULT()
    class ExampleTest : public el::UnitTest {
    public:
        //
        SKIP_BY_DEFAULT()
        void testEveryCombination() {
            // ...
        }
    };

Combine :c:expr:`TAGS(...)`, :c:expr:`TESTED_TARGETS(...)` and :c:expr:`SKIP_BY_DEFAULT()`
------------------------------------------------------------------------------------------

You can combine :c:expr:`TAGS`, :c:expr:`TESTED_TARGETS`, and :c:expr:`SKIP_BY_DEFAULT` for every class and test function by separating them with whitespace:

.. code-block:: cpp

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

This flexibility allows you to define meta information for each test class and function, making it easier to manage and filter tests based on your requirements.

The Macro :c:expr:`SOURCE_LOCATION()`
-------------------------------------

The macro :c:expr:`SOURCE_LOCATION()` is only used when calling the function :cpp:expr:`runWithContext()`

The Macro :c:expr:`ERBSLAND_UNITTEST_MAIN()`
--------------------------------------------

The macro :c:expr:`ERBSLAND_UNITTEST_MAIN()` is a shortcut for the following ``main`` function:

.. code-block:: cpp

    auto main(int argc, char *argv[]) -> int {
      return ::erbsland::unittest::Controller::instance()->main(argc, argv);
    }

