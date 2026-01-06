// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "AssertFailed.hpp"
#include "ConsoleLine.hpp"
#include "SourceLocation.hpp"

#include <format>
#include <functional>
#include <string>
#include <type_traits>


namespace erbsland::unittest {


class UnitTest;


/// @internal
/// The context for a `REQUIRE...` or `CHECK...` evaluation.
///
class AssertContext final {
public:
    /// ctor
    ///
    AssertContext(
        UnitTest *unitTest,
        int flags,
        const char* macroName,
        const char* expression,
        SourceLocation sourceLocation);

    /// dtor
    ///
    ~AssertContext();

public:
    /// The outcome was expected.
    ///
    void expectedResult();

    /// The outcome was unexpected.
    ///
    void unexpectedResult();

    /// There was an unexpected exception.
    ///
    void unexpectedException();

    /// Get this context as string.
    ///
    auto toString() -> std::string;

public:
    UnitTest* unitTest; ///< Unittest for this context.
    int flags; ///< The flags.
    const char *macroName; ///< The current name of the macro evaluating an expression.
    const char *expression; ///< The current expression which was evaluated.
    SourceLocation sourceLocation; ///< The source location.
    std::string exceptionType; ///< The type of the exception
    std::string exceptionMessage; ///< The `what()` message of the exception.
};


/// Executes a test evaluation and handles the result, including exceptions.
///
/// This function is used to evaluate a test expression within a context and
/// appropriately handle the results or exceptions that occur. It invokes the
/// provided function, checks the result, and updates the context based on
/// whether the evaluation produced an expected result, an unexpected result,
/// or an exception.
///
/// @param test The UnitTest instance associated with the context.
/// @param flags Flags to configure or modify the behavior of the evaluation.
/// @param macroName The name of the macro being evaluated.
/// @param expr The string representation of the expression being evaluated.
/// @param loc The source location where the evaluation is being performed.
/// @param func The function that executes the evaluation of the expression.
///
template<typename Func>
void require(
    UnitTest* test,
    const int flags,
    const char* macroName,
    const char* expr,
    const SourceLocation loc,
    Func&& func)
{
    AssertContext ctx{test, flags, macroName, expr, loc};
    try {
        if (func()) {
            ctx.expectedResult();
        } else {
            ctx.unexpectedResult();
        }
    } catch (const AssertFailed&) {
        throw;
    } catch (const std::exception &ex) {
        ctx.exceptionType = std::string(typeid(ex).name());
        ctx.exceptionMessage = std::string(ex.what());
        ctx.unexpectedException();
    } catch (...) {
        ctx.unexpectedException();
    }
}


/// Test if a type can be formatted using `std::format`.
/// 
/// @tparam T The tested type.
/// 
template<typename T>
constexpr bool is_formattable = std::is_default_constructible_v<
    std::formatter<std::remove_cvref_t<T>, char>
>;


/// Generates a detailed error message for a failed comparison.
///
/// @tparam A The type of the first argument.
/// @tparam B The type of the second argument.
/// @param opExpr The textual representation of the comparison operator.
/// @param aExpr The textual representation of the first argument.
/// @param bExpr The textual representation of the second argument.
/// @param aValue The value of the first argument.
/// @param bValue The value of the second argument.
/// @return A detailed error message.
///
template<typename A, typename B>
auto comparisonErrorMessage(
    const std::string_view opExpr,
    const std::string_view aExpr,
    const std::string_view bExpr,
    const A &aValue,
    const B &bValue) -> std::string {

    constexpr std::size_t maxLen = 80;
    std::string result = "Comparison failed: ";

    if constexpr (is_formattable<A>) {
        result += ConsoleLine::utf8SafeString(std::format("{}", aValue), maxLen);
    } else {
        result += aExpr;
    }
    result += ' ';
    result += opExpr;
    result += ' ';
    if constexpr (is_formattable<B>) {
        result += ConsoleLine::utf8SafeString(std::format("{}", bValue), maxLen);
    } else {
        result += bExpr;
    }
    if constexpr (is_formattable<A>) {
        result += std::format(
            "\n  A: {} => {}",
            aExpr,
            ConsoleLine::utf8SafeString(std::format("{}", aValue), maxLen)
        );
    }
    if constexpr (is_formattable<B>) {
        result += std::format(
            "\n  B: {} => {}",
            bExpr,
            ConsoleLine::utf8SafeString(std::format("{}", bValue), maxLen)
        );
    }
    return result;
}


/// Executes a comparison check using a custom comparator, and reports a failure with a custom message if the
/// comparison returns false.
///
/// @tparam CompareFunc A callable returning `bool`; should perform the actual comparison.
/// @tparam MessageFunc A callable returning `std::string`; used to generate the failure message.
/// @param test The UnitTest instance in which this check is running.
/// @param flags Flags that modify assertion behavior.
/// @param macroName The name of the macro that invoked this comparison (e.g. "REQUIRE").
/// @param expr The textual representation of the comparison expression.
/// @param loc The source location where the comparison was invoked.
/// @param compareFunc Callable that performs the comparison; if it returns false, the failure message from
///     `messageFunc` is recorded.
/// @param messageFunc Callable that returns a detailed error message for a false comparison.
///
template<typename CompareFunc, typename MessageFunc>
void requireComparison(
    UnitTest* test,
    const int flags,
    const char* macroName,
    const char* expr,
    const SourceLocation loc,
    CompareFunc&& compareFunc,
    MessageFunc&& messageFunc)
{
    AssertContext ctx{test, flags, macroName, expr, loc};
    try {
        if (compareFunc()) {
            ctx.expectedResult();
        } else {
            ctx.exceptionType = "requireComparison";
            ctx.exceptionMessage = messageFunc();
            ctx.unexpectedResult();
        }
    } catch (const AssertFailed&) {
        throw;
    } catch (const std::exception &ex) {
        ctx.exceptionType = std::string(typeid(ex).name());
        ctx.exceptionMessage = std::string(ex.what());
        ctx.unexpectedException();
    } catch (...) {
        ctx.unexpectedException();
    }
}


/// Verifies that the provided function does not throw any exception. Records success if no exception is thrown,
/// otherwise records the caught exception as an unexpected failure.
///
/// @tparam Func A callable type with no return (void) or whose return value is ignored.
/// @param test The UnitTest instance in which this check is running.
/// @param flags Flags that modify assertion behavior.
/// @param macroName The name of the macro that invoked this check (e.g. "REQUIRE_NO_THROW").
/// @param expr The textual representation of the call or expression expected not to throw.
/// @param loc The source location where the check was invoked.
/// @param func The function or lambda that should execute without throwing.
///
template<typename Func>
void requireNoThrow(
    UnitTest* test,
    const int flags,
    const char* macroName,
    const char* expr,
    const SourceLocation loc,
    Func&& func)
{
    AssertContext ctx{test, flags, macroName, expr, loc};
    try {
        func();
        ctx.expectedResult();
    } catch (const AssertFailed&) {
        throw;
    } catch (const std::exception &ex) {
        ctx.exceptionType = std::string(typeid(ex).name());
        ctx.exceptionMessage = std::string(ex.what());
        ctx.unexpectedResult();
    } catch (...) {
        ctx.unexpectedResult();
    }
}


/// Verifies that the provided function throws _some_ exception. Records success if any exception is thrown;
/// if no exception occurs, records an unexpected failure.
///
/// @tparam Func A callable type that is expected to throw.
/// @param test The `UnitTest` instance in which this check is running.
/// @param flags Flags that modify assertion behavior.
/// @param macroName The name of the macro that invoked this check (e.g. "REQUIRE_THROWS").
/// @param expr The textual representation of the call or expression expected to throw.
/// @param loc The source location where the check was invoked.
/// @param func The function or lambda that should throw.
///
template<typename Func>
void requireThrows(
    UnitTest* test,
    const int flags,
    const char* macroName,
    const char* expr,
    const SourceLocation loc,
    Func&& func)
{
    AssertContext ctx{test, flags, macroName, expr, loc};
    try {
        func();
        ctx.unexpectedResult();
    } catch (const AssertFailed&) {
        throw;
    } catch (...) {
        ctx.expectedResult();
    }
}


/// Verifies that the provided function throws an exception of a specific type. Records success if the thrown
/// exception is exactly `ExceptionClass`; any other exception is treated as unexpected (and its details are captured).
///
/// @tparam ExceptionClass The exception type that must be thrown.
/// @tparam Func A callable type expected to throw `ExceptionClass`.
/// @param test The UnitTest instance in which this check is running.
/// @param flags Flags that modify assertion behavior.
/// @param macroName The name of the macro that invoked this check (e.g. "REQUIRE_THROWS_AS").
/// @param expr The textual representation of the call or expression expected to throw.
/// @param loc The source location where the check was invoked.
/// @param func The function or lambda that should throw @p ExceptionClass.
///
template<typename ExceptionClass, typename Func>
void requireThrowsAs(
    UnitTest* test,
    const int flags,
    const char* macroName,
    const char* expr,
    const SourceLocation loc,
    Func&& func)
{
    AssertContext ctx{test, flags, macroName, expr, loc};
    try {
        func();
        ctx.unexpectedResult();
    } catch (const ExceptionClass&) {
        ctx.expectedResult();
    } catch (const AssertFailed&) {
        throw; \
    } catch (const std::exception &ex) {
        ctx.exceptionType = std::string(typeid(ex).name());
        ctx.exceptionMessage = std::string(ex.what());
        ctx.unexpectedException();
    } catch (...) {
        ctx.unexpectedException();
    }
}


/// Runs the provided function inside an assertion context, capturing any unexpected exceptions as test failures.
/// No boolean result is checked; this is useful when you only care about exception safety.
///
/// @tparam Func A callable type (usually void-returning).
/// @param test The UnitTest instance in which this block is running.
/// @param flags Flags that modify assertion behavior.
/// @param macroName The name of the macro that invoked this block (e.g. "RUN_WITH_CONTEXT").
/// @param expr The textual representation of the code block or expression.
/// @param loc The source location where the block was invoked.
/// @param func The function or lambda to execute; any thrown std::exception or unknown exception is recorded as a failure.
///
template<typename Func>
void runWithContext(
    UnitTest* test,
    const int flags,
    const char* macroName,
    const char* expr,
    const SourceLocation loc,
    Func&& func)
{
    AssertContext ctx{test, flags, macroName, expr, loc};
    try {
        func();
    } catch(const AssertFailed&) {
        throw;
    } catch (const std::exception &ex) {
        ctx.exceptionType = std::string(typeid(ex).name());
        ctx.exceptionMessage = std::string(ex.what());
        ctx.unexpectedException();
    } catch(...) {
        ctx.unexpectedException();
    }
}


}

