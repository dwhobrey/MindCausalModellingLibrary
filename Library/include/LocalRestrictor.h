#pragma once

namespace Plato {

    /// <summary>
    /// A local restrictor that limits operations to the same parent container.
    /// </summary>
    /// DEBUG: TO DO. Does it make sense to change this to ScopeRestrictor and use scopes to restrict operators?
    class LocalRestrictor : public PathRestrictor {
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
        /// <summary>
        /// Gets the class type info for the dynamic class instance type.
        /// </summary>
        virtual const ClassTypeInfo* GetClassTypeInfo() const { return TypeInfo; }

#pragma region // Fields
        /// <summary>
        /// Accessor for a Connector Restrictor that restricts connections to local elements only.
        /// </summary>
        static const LocalRestrictor* LocalConnectorRestrictor;
        /// <summary>
        /// Accessor for a Converter Restrictor that restricts converters to local elements only.
        /// </summary>
        static const LocalRestrictor* LocalConverterRestrictor;
#pragma endregion

        /// <summary>
        /// Constructs a new local Restrictor.
        /// Use LocalRestrictor.LocalXRestrictor to access pre constructed types of restrictors.
        /// </summary>
        /// <param name="operatorRegexTypeName">The type name of the operator this restrictor applies to.</param>
        LocalRestrictor(IdentifierRegex& operatorRegexTypeName);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~LocalRestrictor();

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
