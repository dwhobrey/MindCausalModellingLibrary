#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class Restrictor;
    class ProximityRelator;

    /// <summary>
    /// This restrictor allows an element to control which converters can be applied to its
    /// sub-elements.
    /// </summary>
    class ConverterRestrictor : public Restrictor {
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
        /// <summary>
        /// Gets the class type info for the dynamic class instance type.
        /// </summary>
        virtual const ClassTypeInfo* GetClassTypeInfo() const { return TypeInfo; }

    private:
        /// <summary>
        /// This specifies the pattern of elements to filter against for converter restrictions,
        /// i.e. restrict the application of a converter on a tuple of elements matching this pattern.
        /// </summary>
        ProximityRelator* ElementRelator;

    public:
        /// <summary>
        /// Creates a restrictor that filters the application of converters to a container.
        /// </summary>
        /// <param name="converterRegex">The kinds of converters this restrictor applies to.</param>
        /// <param name="includeWhenMatches">Indicates whether to allow items that pass filter,
        /// or deny items that pass filter.</param>
        /// <param name="scope">The scope of the converter operator to which this restrictor applies.</param>
        /// <param name="relator">A pattern that specifies the pattern of elements to filter against.</param>
        ConverterRestrictor(IdentifierRegex& converterRegex, bool includeWhenMatches, PropertyScopesEnum scope,
            ProximityRelator& relator); 

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~ConverterRestrictor();

        /// <summary>
        /// Checks whether application of the operator is denied.
        /// </summary>
        /// <remarks>This implementation calls the relator's MatchAndValidRelation method.</remarks>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="container">The container to be operated on.</param>
        /// <param name="elements">List of elements to connect.</param>
        /// <returns>Returns true if the operator should not be applied.</returns>
        virtual bool IsDenied(ResolutionModesEnum resolutionMode, Container& container, vector<Property*>& elements);

        /// <summary>
        /// Determines whether application of the operator is allowed
        /// by checking each ConverterRestrictor in the container.
        /// </summary>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="container">The container to be operated on and whose restrictors are to be applied.</param>
        /// <param name="elements">List of elements to connect.</param>
        /// <returns>Returns true if the operator can be applied.</returns>
        static bool IsAllowed(ResolutionModesEnum resolutionMode, Container& container, vector<Property*>& elements);

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
