.. index::
    !single: Text Helpers
    !single: erbsland::unittest::th

Text Helpers
============

The text helper API lives in the :cpp:any:`erbsland::unittest::th` namespace and provides small utilities for
working with encoded text in tests. This includes counting Unicode code-points, converting between string
encodings, generating malformed UTF-8 for validation tests, splitting text into lines, and validating or
comparing text output.

The namespace reference below lists all currently available helpers, including the :cpp:enum:`Utf8Error
<erbsland::unittest::th::Utf8Error>` enum and the :cpp:var:`allUtf8Errors
<erbsland::unittest::th::allUtf8Errors>` convenience constant.

.. doxygennamespace:: erbsland::unittest::th
    :members:

Related Macros
--------------

The following convenience macros are declared together with the text helper API:

.. doxygendefine:: REQUIRE_EQUAL_LINES
.. doxygendefine:: REQUIRE_VALID_UTF8
