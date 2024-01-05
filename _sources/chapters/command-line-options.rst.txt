
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