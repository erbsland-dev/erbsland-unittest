.. index::
    !single: Text Helpers
    !single: erbsland::unittest::th

.. _text-helpers:

Text Helpers
============

The text helper API in :cpp:any:`erbsland::unittest::th` provides practical utilities for working with text in unit tests.

It helps you:

* Count Unicode code-points instead of raw code units.
* Convert between UTF-8, UTF-16, UTF-32, and ``wchar_t`` based strings.
* Produce readable diagnostics for text containing control characters or malformed UTF-8.
* Generate malformed UTF-8 input for validation tests.
* Split text into lines for comparisons.
* Compare multi-line output with simple wildcard support.
* Verify that a byte string contains valid UTF-8.

If you use these helpers frequently, introducing a namespace alias keeps your test code concise:

.. code-block:: cpp

    #include <erbsland/unittest/TextHelper.hpp>

    namespace th = erbsland::unittest::th;

.. warning::

    These helpers are designed for reliability, diagnostics, and convenience — not for raw speed.

    Several functions internally normalize text to UTF-8 or UTF-32 before processing. This makes them robust
    and predictable, but also unsuitable for performance-critical hot paths.

    If your test repeatedly processes large volumes of text in tight loops, prefer a specialized
    implementation that operates directly on your native data representation.

Supported String Types
----------------------

Most helpers provide overloads for the following string view types:

* :cpp:expr:`std::string_view`
* :cpp:expr:`std::u8string_view`
* :cpp:expr:`std::u16string_view`
* :cpp:expr:`std::u32string_view`
* :cpp:expr:`std::wstring_view`

The overloads are intentionally parallel. In most cases, you can call the same helper regardless of the text
type your test already uses.

The :cpp:enum:`Utf8Error <erbsland::unittest::th::Utf8Error>` Enum
------------------------------------------------------------------

The :cpp:enum:`Utf8Error <erbsland::unittest::th::Utf8Error>` enum specifies which malformed UTF-8 sequence
:cpp:func:`invalidUtf8() <erbsland::unittest::th::invalidUtf8()>` should generate.

Available values:

* :cpp:enumerator:`UnexpectedContinuationByte <erbsland::unittest::th::Utf8Error::UnexpectedContinuationByte>`
  A continuation byte without a valid lead byte.
* :cpp:enumerator:`Overlong2ByteSequence <erbsland::unittest::th::Utf8Error::Overlong2ByteSequence>`
  A two-byte overlong encoding.
* :cpp:enumerator:`Truncated2ByteSequence <erbsland::unittest::th::Utf8Error::Truncated2ByteSequence>`
  A two-byte sequence missing its continuation byte.
* :cpp:enumerator:`InvalidContinuationByteIn2ByteSequence <erbsland::unittest::th::Utf8Error::InvalidContinuationByteIn2ByteSequence>`
  A two-byte sequence followed by an invalid continuation byte.
* :cpp:enumerator:`Overlong3ByteSequence <erbsland::unittest::th::Utf8Error::Overlong3ByteSequence>`
  A three-byte overlong encoding.
* :cpp:enumerator:`Truncated3ByteSequence <erbsland::unittest::th::Utf8Error::Truncated3ByteSequence>`
  A three-byte sequence with missing trailing bytes.
* :cpp:enumerator:`InvalidContinuationByteIn3ByteSequence <erbsland::unittest::th::Utf8Error::InvalidContinuationByteIn3ByteSequence>`
  A three-byte sequence containing an invalid continuation byte.
* :cpp:enumerator:`SurrogateCodePoint <erbsland::unittest::th::Utf8Error::SurrogateCodePoint>`
  Bytes that decode to a UTF-16 surrogate code-point (invalid in UTF-8).
* :cpp:enumerator:`Overlong4ByteSequence <erbsland::unittest::th::Utf8Error::Overlong4ByteSequence>`
  A four-byte overlong encoding.
* :cpp:enumerator:`Truncated4ByteSequence <erbsland::unittest::th::Utf8Error::Truncated4ByteSequence>`
  A four-byte sequence with missing trailing bytes.
* :cpp:enumerator:`InvalidContinuationByteIn4ByteSequence <erbsland::unittest::th::Utf8Error::InvalidContinuationByteIn4ByteSequence>`
  A four-byte sequence containing an invalid continuation byte.
* :cpp:enumerator:`CodePointBeyondUnicodeRange <erbsland::unittest::th::Utf8Error::CodePointBeyondUnicodeRange>`
  A sequence above ``U+10FFFF``.
* :cpp:enumerator:`InvalidStartByte <erbsland::unittest::th::Utf8Error::InvalidStartByte>`
  A byte that is not valid as a UTF-8 start byte.

The :cpp:var:`allUtf8Errors <erbsland::unittest::th::allUtf8Errors>` Constant
------------------------------------------------------------------------------

The :cpp:var:`allUtf8Errors <erbsland::unittest::th::allUtf8Errors>` constant contains all enum values in a
single iterable array.

This makes it easy to write exhaustive tests:

.. code-block:: cpp

    for (const auto error : th::allUtf8Errors) {
        auto malformed = th::invalidUtf8(error, "pre", "suf");
        CHECK_FALSE(myUtf8Validator(malformed));
    }

Use this pattern when you want to ensure your UTF-8 handling rejects *all* supported malformed cases.

The :cpp:func:`characterCount() <erbsland::unittest::th::characterCount()>` Function
------------------------------------------------------------------------------------

Use :cpp:func:`characterCount() <erbsland::unittest::th::characterCount()>` to count Unicode code-points
instead of bytes or UTF code units.

.. code-block:: cpp

    auto count1 = th::characterCount("abc");
    auto count2 = th::characterCount("abc→😀");

    REQUIRE_EQUAL(count1, 3);
    REQUIRE_EQUAL(count2, 5);

This is especially useful when your test data contains non-ASCII characters and
:cpp:expr:`std::string::size()` would give misleading results.

.. note::

    The result counts Unicode code-points, not grapheme clusters. A single visible character may still
    consist of multiple code-points.

The :cpp:func:`toConsoleSafeString() <erbsland::unittest::th::toConsoleSafeString()>` Function
-----------------------------------------------------------------------------------------------

This helper converts text into a representation that is safe and readable in console output.

It escapes:

* control characters (newline, carriage return, tab),
* invalid UTF-8 bytes,
* non-ASCII code-points,
* backslashes and double quotes.

If the result contains spaces or escape sequences, it is automatically wrapped in double quotes.

.. code-block:: cpp

    auto text = th::toConsoleSafeString("abc\n\r\t\\\"\u1234", 100);
    REQUIRE_EQUAL(text, R"("abc\n\r\t\\\"\u{1234}")");

Use this helper whenever raw strings would make diagnostics hard to interpret.

The ``maxLength`` parameter limits the length of the *escaped* result. If the output is truncated, a suffix
like ``(... +<count> more)`` is appended.

The :cpp:func:`toStdString() <erbsland::unittest::th::toStdString()>` Function
-------------------------------------------------------------------------------

This function converts supported string types into a UTF-8 encoded :cpp:expr:`std::string`.

* :cpp:expr:`std::string_view` and :cpp:expr:`std::u8string_view` are copied as-is.
* :cpp:expr:`std::u16string_view`, :cpp:expr:`std::u32string_view`, and :cpp:expr:`std::wstring_view` are
  transcoded to UTF-8.

Use this to normalize text across platforms before comparing or printing it.

.. important::

    For :cpp:expr:`std::string_view`, the function does **not** validate UTF-8. It simply copies the bytes.

    If you need validation, call
    :cpp:func:`requireValidUtf8() <erbsland::unittest::th::requireValidUtf8()>` explicitly.

The :cpp:func:`toStdU32String() <erbsland::unittest::th::toStdU32String()>` Function
-------------------------------------------------------------------------------------

This function converts supported input into a UTF-32 :cpp:expr:`std::u32string`.

Use it when you want one Unicode code-point per element.

For UTF-8 input, malformed sequences are replaced with the Unicode replacement character ``U+FFFD`` instead
of causing errors.

This makes the function safe for diagnostics and normalization, even when the input is not fully trustworthy.

The Hex Parsing Functions
-------------------------

These helpers parse hexadecimal text into strings with fixed-width element types:

* :cpp:func:`stdStringFromHex() <erbsland::unittest::th::stdStringFromHex()>`
* :cpp:func:`stdU8StringFromHex() <erbsland::unittest::th::stdU8StringFromHex()>`
* :cpp:func:`stdU16StringFromHex() <erbsland::unittest::th::stdU16StringFromHex()>`
* :cpp:func:`stdU32StringFromHex() <erbsland::unittest::th::stdU32StringFromHex()>`
* :cpp:func:`stdWStringFromHex() <erbsland::unittest::th::stdWStringFromHex()>`

Use them when expressing test data as hex is clearer than using source code string literals.

Format Rules for Hex Parsing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All hex parsing helpers follow the same rules:

* Spaces are optional.
* Spaces may only appear *between* complete groups.
* Leading and trailing spaces are not allowed.
* Invalid hex digits throw :cpp:expr:`std::logic_error`.

The :cpp:func:`invalidUtf8() <erbsland::unittest::th::invalidUtf8()>` Function
-------------------------------------------------------------------------------

This helper intentionally creates malformed UTF-8 sequences.

Use it to test validators, decoders, or error handling:

.. code-block:: cpp

    auto sample = th::invalidUtf8(th::Utf8Error::InvalidStartByte, "before ", " after");
    CHECK_FALSE(myUtf8Validator(sample));

Combining it with :cpp:var:`allUtf8Errors <erbsland::unittest::th::allUtf8Errors>` allows you to build
exhaustive negative tests with very little code.

The :cpp:func:`splitLineViews() <erbsland::unittest::th::splitLineViews()>` Function
-------------------------------------------------------------------------------------

Splits text at ``\\n`` and returns views into the original string.

Use this when you want to inspect lines without copying data.

.. important::

    The returned views reference the original input. Do not modify or destroy the source string while they are
    in use.

The :cpp:func:`splitLines() <erbsland::unittest::th::splitLines()>` Function
-----------------------------------------------------------------------------

Same behavior as :cpp:func:`splitLineViews() <erbsland::unittest::th::splitLineViews()>`, but returns owned
strings instead of views.

Use this when you need independence from the original input lifetime.

The :cpp:func:`requireEqualLines() <erbsland::unittest::th::requireEqualLines()>` Function
-------------------------------------------------------------------------------------------

Compares two sequences of lines and produces a side-by-side diff on failure.

The expected lines support simple wildcards:

* ``*`` matches any number of characters.
* ``?`` matches exactly one character.

Use this for stable comparisons of output that may contain variable fragments.

.. code-block:: cpp

    void testRequireEqualLines() {
        const auto expected = std::vector<std::string_view>{
            "hello one two three",
            "anything*",
            "*anything",
            "anything*anything",
            "two??wildcards",
            "*any??",
        };
        const auto actual = std::vector<std::string_view>{
            "hello one two three",
            "anything→goes.here",
            "another line with anything",
            "anything can be in the middle, anything",
            "two+-wildcards",
            "is there any::",
        };
        // manual use
        WITH_CONTEXT(th::requireEqualLines(*this, actual, expected));
        // macro use
        REQUIRE_EQUAL_LINES(actual, expected);
    }

If the two line sequences don't match, you will get diagnostic output like this:

.. code-block:: text

    | Actual (2)                  |     | Expected (3)                |
    |-----------------------------|-----|-----------------------------|
    | one two three four five six | === | one two three four five six |
    | different                   |  X  | another line                |
    |                             |  X  | last line                   |
    |-----------------------------|-----|-----------------------------|

The :c:macro:`REQUIRE_EQUAL_LINES` Macro
----------------------------------------

Convenience wrapper around :cpp:func:`requireEqualLines()` that automatically integrates with the test
context:

.. code-block:: cpp

    REQUIRE_EQUAL_LINES(actualLines, expectedLines);

The :cpp:func:`requireValidUtf8() <erbsland::unittest::th::requireValidUtf8()>` Function
-----------------------------------------------------------------------------------------

Validates that a string contains well-formed UTF-8 and only valid Unicode scalar values.

On failure, the test reports:

* the byte position of the error,
* a readable preview of the input.

Rejected cases include:

* malformed byte sequences,
* truncated sequences,
* surrogate code-points,
* code-points above ``U+10FFFF``.

Use this when correctness of UTF-8 encoding is part of what your test verifies.

The :c:macro:`REQUIRE_VALID_UTF8` Macro
---------------------------------------

Convenience macro for :cpp:func:`requireValidUtf8()`:

.. code-block:: cpp

    REQUIRE_VALID_UTF8(resultText);

For the full API reference, see :doc:`reference/text-helpers`.
