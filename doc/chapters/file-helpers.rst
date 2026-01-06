.. index::
    !single: File Helpers
    !single: erbsland::unittest::fh

.. _file-helpers:

File Helpers
============

This library provides a small but very practical set of helper functions for working with test data files and file paths in unit tests. All helpers live in the :cpp:any:`erbsland::unittest::fh` namespace and are designed to remove platform- and build-system-specific quirks from your test code.

If you want to keep your test code concise, you can import the namespace and use the :cpp:any:`fh <erbsland::unittest::fh>` shortcut. Of course, using the fully qualified names works just as well.

.. code-block:: cpp

    #include <erbsland/unittest/FileHelper.hpp>

    namespace fh = erbsland::unittest::fh;

The following sections describe each helper function in detail and show common usage patterns.

The :cpp:func:`unitTestExecutablePath() <erbsland::unittest::fh::unitTestExecutablePath()>` Function
----------------------------------------------------------------------------------------------------

Returns the absolute path to the currently running unit test executable.

This is mainly useful if you need to derive paths relative to the test binary itself, for example when debugging path issues or when interacting with external tools that expect absolute paths.

The :cpp:func:`resolveDataPath() <erbsland::unittest::fh::resolveDataPath()>` Function
--------------------------------------------------------------------------------------

Resolves the absolute path to a test data file or directory.

This function is especially helpful when your test data is copied into the build directory using the ``COPY_TEST_DATA`` option of the ``erbsland_unittest`` CMake macro (see :doc:`cmake-integration`). It transparently handles the differences between single-config and multi-config generators (such as MSVC), so your tests behave consistently across platforms and build setups.

.. code-block:: cpp

    auto dataPath = fh::resolveDataPath("data/CategoryContainsTestData.txt");

If the specified file or directory cannot be found, the test is aborted with a clear and descriptive error message. This makes configuration problems visible immediately instead of causing subtle test failures later on.

The :cpp:func:`readDataText() <erbsland::unittest::fh::readDataText()>` Function
--------------------------------------------------------------------------------

Resolves the given data path and reads the complete contents of the file into a single string.

This helper is ideal for tests that operate on small to medium-sized text files and want to compare or parse their contents in one step.

.. code-block:: cpp

    auto content = fh::readDataText("data/test_file.txt");

An error is thrown if the file does not exist or if its size exceeds the configured maximum size (10 MB by default). This protects your tests from accidentally loading very large files.

The :cpp:func:`readDataLines() <erbsland::unittest::fh::readDataLines()>` Function
----------------------------------------------------------------------------------

Similar to :cpp:func:`readDataText()`, but reads the file line by line and returns a vector of strings, one entry per line.

This function is well suited for configuration files, line-based formats, or tests where individual lines need to be inspected separately.

.. code-block:: cpp

    auto lines = fh::readDataLines("data/config.ini");
