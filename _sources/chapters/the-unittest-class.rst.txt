
.. index::
    !single: UnitTest

.. cpp:namespace:: erbsland::unittest::UnitTest

.. _the-unittest-class:

The :cpp:expr:`UnitTest` Class
==============================

The :cpp:expr:`UnitTest` class provides the following public interface that can be utilized:

.. code-block:: cpp

    namespace erbsland::unittest {

    class UnitTest {
    public:
        virtual auto additionalErrorMessages() -> std::string;
        virtual void setUp();
        virtual void tearDown();
        void runWithContext(
            const SourceLocation &sourceLocation,
            const std::function<void()> &testFn,
            const std::function<std::string()> &diagnoseFn = nullptr);
        void consoleWriteLine(const std::string &text);
        auto unitTestExecutablePath() -> std::filesystem::path;
    };

    }

The :cpp:expr:`additionalErrorMessages()` Function
--------------------------------------------------

Implement the :cpp:expr:`additionalErrorMessages()` function to provide more details about the state of instance variables in case a test fails:

.. code-block:: cpp

    class ContextTest : public el::UnitTest {
    public:
        ExampleLib exampleLib{};

        auto additionalErrorMessages() -> std::string override {
            try {
                std::string text;
                text += "ExampleLib:\n";
                text += std::format("    getName() = \"{}\"\n", exampleLib.getName());
                text += std::format("    getNameLength() = {}\n", exampleLib.getNameLength());
                return text;
            } catch(...) {
                return {"Unexpected Exception"};
            }
        }

        void setAndVerifyName(const std::string &name) {
            exampleLib.setName(name);
            auto expectedSize = name.size();
            REQUIRE(exampleLib.getName() == name);
            REQUIRE(exampleLib.getNameLength() == expectedSize);
        }
    };

This example uses an instance variable ``exampleLib`` for the tested object and tries to return additional details about the state of the object when a test failed.

.. code-block:: cpp

    class LongTest : public el::UnitTest {
    public:
        // ...
        bool result{};
        std::string name{};

        auto additionalErrorMessages() -> std::string override {
            try {
                std::string text;
                text += std::format("result = {}\n", result);
                text += std::format("name = \"{}\"\n", name);
                text += std::format("expected result = {}\n", expectedResult());
                return text;
            } catch(...) {
                return {"Unexpected Exception"};
            }
        }
        // ... tests ...
    };

This example does not store the tested instance, but the test input and result as instance variables. If a test fails, the last state of the input and result is returned.

*It is important to catch every exception that could have been thrown by the implementation of this function. It ensures that you always get a meaningful output, even when the additional information cannot be extracted.*

The :cpp:expr:`setUp()` and :cpp:expr:`tearDown()` Methods
----------------------------------------------------------

Overwrite these methods to execute code before and after *every* test function in the test suite. We do not recommend using these methods and provide them only for people that are used to working with them.

If an object must be prepared for each test, we recommend writing a function like ``createExampleObject()`` that creates an instance of the tested object for the test function.

The :cpp:expr:`runWithContext()` Method
---------------------------------------

If you like to return additional context information of local variables, you can call this function as shown in the next example:

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

The :cpp:expr:`consoleWriteLine()` Method
-----------------------------------------

If you are searching for problems in your tests, or like to provide status information on long-running tests, you can call this method from within your test methods to produce additional output on the console.

.. code-block:: cpp

    void testIsNamePalindromeBruteForce() {
        auto exampleLib = ExampleLib{};
        name = std::string(size, 'a');
        std::size_t count{0};
        while (name[size-1] != 'z') {
            exampleLib.setName(name);
            result = exampleLib.isNamePalindrome();
            REQUIRE(result == expectedResult());
            for (std::size_t i = 0; i < size; ++i) {
                if (name[i] < 'z') {
                    name[i] += 1;
                    break;
                } else {
                    name[i] = 'a';
                }
            }
            if (count % 1'000'000 == 0) {
                auto text = std::ostringstream{};
                text << "Testing name " << count << ": " << name;
                consoleWriteLine(text.str());  // <------------------------------
            }
            count += 1;
        }
    }

The :cpp:expr:`unitTestExecutablePath()` Method
-----------------------------------------------

If you work with test data that is stored along the unit test executable, you can call this method to get the path of the currently running unittest.

