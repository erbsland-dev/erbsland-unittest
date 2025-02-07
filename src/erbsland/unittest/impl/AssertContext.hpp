// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "AssertFailed.hpp"
#include "SourceLocation.hpp"

#include <functional>
#include <string>


namespace erbsland::unittest {


class UnitTest;


/// @internal
/// The context for an assert (require, check) evaluation.
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


template<typename Func>
inline void require(
    UnitTest* test,
    int flags,
    const char* macroName,
    const char* expr,
    SourceLocation loc,
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


template<typename Func>
inline void requireNoThrow(
    UnitTest* test,
    int flags,
    const char* macroName,
    const char* expr,
    SourceLocation loc,
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


template<typename Func>
inline void requireThrows(
    UnitTest* test,
    int flags,
    const char* macroName,
    const char* expr,
    SourceLocation loc,
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


template<typename ExceptionClass, typename Func>
inline void requireThrowsAs(
    UnitTest* test,
    int flags,
    const char* macroName,
    const char* expr,
    SourceLocation loc,
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


template<typename Func>
inline void runWithContext(
    UnitTest* test,
    int flags,
    const char* macroName,
    const char* expr,
    SourceLocation loc,
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

