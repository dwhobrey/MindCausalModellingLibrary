#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class ProximityRelator;

    /// <summary>
    /// A Converter is a means of adapting the context of elements.
    /// After elements have been added to a region, but before they are connected,
    /// a check is done to determine whether any converters are needed between elements.
    /// A converter generates elements and connection rules for interfacing elements
    /// of various names, types, or proximity.
    /// Users define their own converters and add them to a region.
    /// </summary>
    /// <remarks>
    /// <para>
    /// A converter is applied when a set of elements in a
    /// container matches the N-arity tuple of element kinds specified in the converter relator.
    /// A converter may be applied multiple times to each set of elements in a container
    /// that matches the relator.
    /// </para>
    /// <para>
    /// Converter rule, if final connections are needed resulting from a converter,
    /// the final connectors should be added to the converter.
    /// </para>
    /// </remarks>
    class Converter : public Container {
#pragma region // Class variables.
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
        /// The kinds of properties this container can hold.
        /// </summary>
        static const ClassTypeInfo::HashSet* PropertyTypes;
#pragma endregion

        /// <summary>
        /// The phase in the configuration process that this converter should be applied.
        /// </summary>
        ResolutionModesEnum ResolutionMode;

        /// <summary>
        /// The relator that determines the elements this converter applies to.
        /// </summary>
        ProximityRelator* ElementRelator;

        /// <summary>
        /// Construct a new converter.
        /// </summary>
        /// <param name="name">Name of converter.</param>
        /// <param name="scope">The application range of this converter.</param>
        /// <param name="resolutionMode">The phase in the configuration process when this converter should be applied.</param>
        /// <param name="elementRelator">A proximity relator that specifies the list of N element kinds that this converter
        /// can connect in an N-arity relational sense.
        /// </param>
        Converter(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionMode, ProximityRelator& elementRelator);

        /// <summary>
        /// Apply the converter to all matching element tuples found in the parent container.
        /// </summary>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The container of elements to check for application of this converter.</param>
        virtual void Apply(ResolutionModesEnum resolutionMode, Container& parent);

    protected:
        /// <summary>
        /// Add conversion rules (etc) to owner element as necessary
        /// in order to connect elements.
        /// User derived class would then add sub elements as necessary.
        /// </summary>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent element in which the elements are to be connected.</param>
        /// <param name="elements">List of elements to connect.</param>
        virtual void Apply(ResolutionModesEnum resolutionMode, Container& parent, vector<Property*> & elements);

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
