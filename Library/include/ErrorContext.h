#pragma once

namespace Plato {

    class ErrorMessage;

    /// <summary>
    /// Each model has an ErrorContext object, which records any errors via an error message stack.
    /// </summary>
    class ErrorContext {
    public:
        /// <summary>
        /// The default size to make the error stack.
        /// </summary>
        static const int DefaultStackSize = 10;

        /// <summary>
        /// The exit code returned by the model.
        /// </summary>
        int ExitCode;

    private:
        /// <summary>
        /// The number of errors to keep in the model's error stack.
        /// </summary>
        int mStackSize;

        /// <summary>
        /// A run-time stack of error messages is built up as errors occur.
        /// This is used for subsequent error code to error message conversion
        /// when required.
        /// </summary>
        vector<ErrorMessage*>* mErrorMessageStack;

    public:
        /// <summary>
        /// Creates a new error context for tracking error messages and exit code.
        /// </summary>
        /// <param name="stackSize">The maximum number of error messages to keep on stack.
        /// If stackSize is negative, then the default value is used.</param>
        ErrorContext(int stackSize);

        /// <summary>
        /// Frees up the error context.
        /// </summary>
        ~ErrorContext();

        /// <summary>
        /// Returns the current number of error messages on the stack.
        /// </summary>
        int Count();

        /// <summary>
        /// Adds an error message to the error message stack.
        /// </summary>
        /// <param name="errorMessage">The error message to add.</param>
        void Add(ErrorMessage& errorMessage);

        /// <summary>
        /// Fetch the ErrorMessage associated with the error code.
        /// Searches the error stack for the first match on error code.
        /// </summary>
        /// <param name="errorCode">Error code to find in error message stack.</param>
        /// <returns>Returns the associated ErrorMessage if the error code is on the stack, otherwise NULL.</returns>
        ErrorMessage* CodeToMessage(int errorCode);

        /// <summary>
        /// Produce a textual enumeration of the error stack.
        /// </summary>
        /// <remarks>The caller must free the string</remarks>
        /// <returns>Returns a text dump of the error stack.</returns>
        string& StatusReport();
    };
}
