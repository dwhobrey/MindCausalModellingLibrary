#pragma once

namespace Plato {

    /// <summary>
    /// Holds details on an error in the Error Stack.
    /// </summary>
    class ErrorMessage {
    public:
        /// <summary>
        /// The error code of this error.
        /// </summary>
        int mErrorCode;

        /// <summary>
        /// The error message.
        /// </summary>
        /// <remarks>Owned by this class.</remarks>
        const string* mErrorMessage;

        /// <summary>
        /// Called by Error when pushing errors on the stack.
        /// </summary>
        /// <param name="errorCode">The error code for this error.</param>
        /// <param name="errorMessage">The error message.</param>
        ErrorMessage(int errorCode, const string& errorMessage);

        /// <summary>
        /// Free up the error message.
        /// </summary>
        ~ErrorMessage();

        /// <summary>
        /// Produce a text representation for the error message.
        /// </summary>
        /// <returns>Returns the error code and message as a string.</returns>
        string& StatusReport();
    };
}
