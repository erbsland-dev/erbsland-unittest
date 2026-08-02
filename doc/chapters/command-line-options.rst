
.. _command-line:
.. index::
    !single: Command Line Options
    single: Command Line
    single: Options

Command Line Options for the Unit Test Executable
=================================================

The unit test executable supports the following command-line arguments:

.. option:: -h, --help

    Display help information.

.. option:: -v, --verbose

   Display verbose messages, including skipped tests.

.. option:: -e

   Stop at the first error.

.. option:: -l, --list

   List all test suites and tests without running them.

.. option:: -c, --no-color

   Disable colorized output and status updates.

.. option:: -s, --no-summary

   Do not list the first three errors at the end of the test run.

.. option:: -t, --time-tests

   Measure and display the runtime of each test and print method. The duration is displayed immediately before the
   method status. After each suite, its total wall-clock runtime is displayed on a separate line.

.. option:: --time-stat

   Collect test and suite timings and display time statistics after the final test result. The report contains totals,
   averages, medians, and the ten slowest tests and suites. Print methods are excluded from test statistics.

.. option:: name:<name>

   Run only tests with the specified test or class name (case-sensitive).

.. option:: +name:<name>

   Include tests with the specified test or class name, even if they are optional.

.. option:: -name:<name>

   Exclude tests with the specified test or class name.

.. option:: target:<target>

   Run only tests for the specified target.

.. option:: +target:<target>

   Include tests for the specified target, even if they are optional.

.. option:: -target:<target>

   Exclude tests for the specified target.

.. option:: tag:<tag>

   Run only tests with the specified tags.

.. option:: +tag:<tag>

   Include tests with the specified tags, even if they are optional.

.. option:: -tag:<tag>

   Exclude tests with the specified tags.

By default, all tests that are not marked with :c:expr:`SKIP_BY_DEFAULT()` are enabled. You can individually add (``+``) or remove (``-``) tests from this initial set using the options above.

When specifying one or more options like ``<opt>:<tag>``, only tests with the given tags are enabled. Further ``+/-`` options will modify this set.

The processing order of the options is `<opt>`, `+<opt>`, `-<opt>` and does not depend on their order on the command line. Therefore, the `-` options always have the highest priority and will skip the specified tests regardless of any other options.

Timing Tests and Suites
-----------------------

The timing options are independent. Use :option:`--time-tests` for timings in the regular test list,
:option:`--time-stat` for the final statistics, or combine them to display both forms. Timings use milliseconds,
seconds, minutes, or hours depending on their length and are formatted with three significant digits.

Test timing includes setup, the test or print method, and teardown. Suite timing covers the complete suite run,
including construction and reporting overhead. Failed or otherwise interrupted entries are retained in statistics and
marked as interrupted. When no timing option is present, the existing output remains unchanged.
