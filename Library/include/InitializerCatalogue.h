#pragma once

namespace Plato {
    /// <summary>
    /// Holds method pointers for a class's initializer and finalizer.
    /// </summary>
    class InitializerCatalogueEntry {
    public:
        /// <summary>
        /// A class's initializer and finalizer methods must have this signature.
        /// </summary>
        typedef void (*InitializerDelegate)();
        /// <summary>
        /// A pointer to a class's intializer method.
        /// </summary>
        InitializerDelegate Initializer;
        /// <summary>
        /// A pointer to a class's finalizer method.
        /// </summary>
        InitializerDelegate Finalizer;
        /// <summary>
        /// Construct a new object for holding initializer details for a class.
        /// </summary>
        /// <param name="initializer">A pointer to a class's intializer method.</param>
        /// <param name="finalizer">A pointer to a class's finalizer method.</param>
        InitializerCatalogueEntry(InitializerDelegate initializer, InitializerDelegate finalizer);
    };

    /// <summary>
    /// This class is used to initialise global static objects in a set order.
    /// </summary>
    class InitializerCatalogue {
        friend class Model;
    private:
        /// <summary>
        /// The library's table of classes requiring initialisation.
        /// </summary>
        /// <remarks>
        /// Classes that require initialisation during library configuration should
        /// add an entry to this table via the InitInitializerCatalogueTable method.
        /// The table must be in dependency order.
        /// </remarks>
        static vector<InitializerCatalogueEntry*>* InitializerCatalogueTable;
        /// <summary>
        /// Initializer for the InitializerCatalogueTable.
        /// </summary>
        /// <remarks>
        /// Classes requiring initialisation should add an entry to this method.
        /// This method also calls each class Initializer in the order they were added to the table.
        /// </remarks>
        static void Initialize();

        /// <summary>
        /// Calls, in reverse order, the Finalizer methods of all entries in the IntializerCatalogueTable and
        /// then deletes the table.
        /// </summary>
        static void Finalize();
    };
}
