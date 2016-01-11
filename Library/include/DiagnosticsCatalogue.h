#pragma once

namespace Plato {
    /// <summary>
    /// This class is used to generate a simple developer maintained table of
    /// classes, implementing unit tests, that should be called during diagnostics.
    /// </summary>
    class DiagnosticsCatalogue {
    public:
        /// <summary>
        /// The library's table of classes implementing unit tests.
        /// </summary>
        /// <remarks>
        /// Classes that perform diagnostics during library configuration should
        /// add an entry to this table via the Initializer.
        /// The table must be in alphabetical order.
        /// </remarks>
        static vector<DiagnosticsCatalogueEntry*>* DiagnosticsCatalogueTable;

    private:
        friend class InitializerCatalogue;
        /// <summary>
        /// The class initializer.
        /// </summary>
        static void Initializer();
        /// <summary>
        /// The class finalizer.
        /// </summary>
        static void Finalizer();
    };
}
