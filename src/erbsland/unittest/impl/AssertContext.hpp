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
class AssertContext
{
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

public: // function execution in context
    inline void require(const std::function<bool()> &func) {
        try {
            if (func()) {
                expectedResult();
            } else {
                unexpectedResult();
            }
        } catch (const AssertFailed&) {
            throw;
        } catch (const std::exception &ex) {
            exceptionType = std::string(typeid(ex).name());
            exceptionMessage = std::string(ex.what());
            unexpectedException();
        } catch (...) {
            unexpectedException();
        }
    }

    inline void requireNoThrow(const std::function<void()> &func) {
        try {
            func();
            expectedResult();
        } catch (const AssertFailed&) {
            throw;
        } catch (const std::exception &ex) {
            exceptionType = std::string(typeid(ex).name());
            exceptionMessage = std::string(ex.what());
            unexpectedResult();
        } catch (...) {
            unexpectedResult();
        }
    }

    inline void requireThrows(const std::function<void()> &func) {
        try {
            func();
            unexpectedResult();
        } catch (const AssertFailed&) {
            throw;
        } catch (...) {
            expectedResult();
        }
    }

    template<typename ExceptionClass>
    inline void requireThrowsAs(const std::function<void()> &func) {
        try {
            func();
            unexpectedResult();
        } catch (const ExceptionClass&) {
            expectedResult();
        } catch (const AssertFailed&) {
            throw; \
        } catch (const std::exception &ex) {
            exceptionType = std::string(typeid(ex).name());
            exceptionMessage = std::string(ex.what());
            unexpectedException();
        } catch (...) {
            unexpectedException();
        }
    }

    inline void runWithContext(const std::function<void()> &func) {
        try {
            func();
        } catch(const AssertFailed&) {
            throw;
        } catch (const std::exception &ex) {
            exceptionType = std::string(typeid(ex).name());
            exceptionMessage = std::string(ex.what());
            unexpectedException();
        } catch(...) {
            unexpectedException();
        }
    }

public:
    UnitTest* unitTest; ///< Unittest for this context.
    int flags; ///< The flags.
    const char *macroName; ///< The current name of the macro evaluating an expression.
    const char *expression; ///< The current expression which was evaluated.
    SourceLocation sourceLocation; ///< The source location.
    std::string exceptionType; ///< The type of the exception
    std::string exceptionMessage; ///< The `what()` message of the exception.
};


}

