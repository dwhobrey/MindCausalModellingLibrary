#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class IdentifierRegex;
    class Property;
    class Container;

    /// <summary>
    /// An element can add filters, via a Restrictor, that specify which operations
    /// may be performed on it.
    /// </summary>
    /// <remarks>
    /// For example, connection restrictors allow elements to control what other elements
    /// may connect to it:
    /// Local connections would be specified via: "./" + Path.WildPattern;
    /// Global connections would be specified via: "/" + Path.AnyPattern;
    /// The type of the connection, input or output, would be specified by
    /// terminating the filter pattern with the appropriate name, e.g. "input" or "output".
    /// </remarks>
    class Restrictor : public Property {
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
        /// <summary>
        /// Gets the class type info for the dynamic class instance type.
        /// </summary>
        virtual const ClassTypeInfo* GetClassTypeInfo() const { return TypeInfo; }

        /// <summary>
        /// Indicates whether to allow items that pass filter,
        /// or deny items that pass filter.
        /// </summary>
        bool IncludeWhenMatches;

        /// <summary>
        /// The scope of the operator to which this restrictor applies.
        /// </summary>
        PropertyScopesEnum OperatorScope;

        /// <summary>
        /// A regex identifier used to specify the operator name and type pattern that this restrictor applies to.
        /// </summary>
        IdentifierRegex* OperatorRegex;

        /// <summary>
        /// General constructor for a new Restrictor.
        /// </summary>
        /// <param name="creator">The container that created this connector.</param>
        /// <param name="parent">The hierarchical parent of this connector.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        /// <param name="operatorRegex">A regex Identifier that specifies the name and type pattern
        /// of the operator this restrictor applies to.</param>
        /// <param name="includeWhenMatches">Indicates whether to allow items that pass filter,
        /// or deny items that pass filter.</param>
        /// <param name="operatorScope">The scope of the operator to which this restrictor applies.</param>
        /// DEBUG: TO DO. Needs a unique name because multiple restrictors may be added for the same operator.
        Restrictor(Container* creator, Container* parent, Identifier & elementName, PropertyScopesEnum scope, 
            IdentifierRegex& operatorRegex, bool includeWhenMatches, PropertyScopesEnum operatorScope);

        /// <summary>
        /// Standard constructor for a new Restrictor based on the given operator and filter type.
        /// </summary>
        /// <param name="operatorRegex">A regex Identifier that specifies the name and type pattern
        /// of the operator this restrictor applies to.</param>
        /// <param name="includeWhenMatches">Indicates whether to allow items that pass filter,
        /// or deny items that pass filter.</param>
        /// <param name="operatorScope">The scope of the operator to which this restrictor applies.</param>
        /// DEBUG: TO DO. Needs a unique name because multiple restrictors may be added for the same operator.
        Restrictor(IdentifierRegex& operatorRegex, bool includeWhenMatches, PropertyScopesEnum operatorScope);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Restrictor();

        /// <summary>
        /// Checks whether application of the operator is denied.
        /// </summary>
        /// <remarks>This default implementation simply returns false.</remarks>
        /// <param name="configureOperator">The operator to check whether application is denied.</param>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="container">The container to be operated on.</param>
        /// <returns>Returns true if the operator should not be applied.</returns>
        virtual bool IsDenied(Property& configureOperator, ResolutionModesEnum resolutionMode, Container& container);

        /// <summary>
        /// Determines whether application of the operator is allowed
        /// by checking each restrictor in the parent container.
        /// </summary>
        /// <param name="configureOperator">The operator to check.</param>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="container">The container to be operated on.</param>
        /// <returns>Returns true if the operator can be applied.</returns>
        static bool IsAllowed(Property& configureOperator, ResolutionModesEnum resolutionMode, Container& container);

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
