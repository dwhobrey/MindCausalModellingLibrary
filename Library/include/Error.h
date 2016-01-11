#pragma once
namespace Plato {

    class ClassTypeInfo;
    class ErrorContext;

    /// <summary>
    /// Manages error codes and messages.
    /// </summary>
    class Error {
    public:
        /// <summary>
        /// When set, this forces all error logging to be echoed to the console.
        /// </summary>
        static bool EchoFlag;

        /// <summary>
        /// Insert an error message into the error message stack
        /// for the current model and then return the error code.
        /// </summary>
        /// <param name="echoToConsole">If true, message is also sent to the console. A newline is appended to the output.</param>
        /// <param name="errorContext">If not NULL, the message is added to the context's error stack.</param>
        /// <param name="classInfo">The class type info for the class in which the error occurred.</param>
        /// <param name="methodName">The name of the method in which the error occurred.</param>
        /// <param name="errorIndex">The error number relative to the enclosing method.</param>
        /// <param name="errorFormatMessage">The error format message to associate with this code.</param>
        /// <param name="...">The arguments to apply to the format message.</param>
        /// <returns>Returns the error code for the message.</returns>
        static int Log(bool echoToConsole, ErrorContext& errorContext, const ClassTypeInfo& classInfo,
            const char* methodName, int errorIndex, const char* errorFormatMessage, ...);
    };
}
