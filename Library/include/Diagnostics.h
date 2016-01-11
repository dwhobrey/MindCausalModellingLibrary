#pragma once

namespace Plato {

    /// <summary>
    /// Holds details on a class that can perform diagnostics.
    /// </summary>
    class DiagnosticsCatalogueEntry {
    public:
        /// <summary>
        /// The static class method called to perform diagnostics on class.
        /// </summary>
        /// <param name="message">Result of test.</param>
        /// <returns>Returns the number of failures, i.e. zero on success.</returns>
        typedef int (*ConductUnitTestDelegate)(string& message);
        /// <summary>
        /// Name of class that can perform diagnostics.
        /// </summary>
        const string& mClassName;
        /// <summary>
        /// The static unit test method for the class.
        /// </summary>
        ConductUnitTestDelegate mUnitTestDelegate;
        /// <summary>
        /// Constructs a new entry for the Diagnostics Catalogue.
        /// </summary>
        /// <param name="className">Name of class that can perform diagnostics.</param>
        /// <param name="unitTestDelegate">The static unit test method for the class.</param>
        DiagnosticsCatalogueEntry(const string& className, ConductUnitTestDelegate unitTestDelegate);
    };

    /// <summary>
    /// Runs diagnostics on the Library.
    /// </summary>
    class Diagnostics {
    public:
        /// <summary>
        /// Conduct diagnostics on the library.
        /// </summary>
        /// <returns>Returns 0 if ok, otherwise error code.</returns>
        static int ConductTests();
    };
}
