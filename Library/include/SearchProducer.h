#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class IdentifierRegex;
    class Property;
    class Container;
    class Producer;

    /// <summary>
    /// A search producer looks up the container hierarchy for a producer.
    /// </summary>
    class SearchProducer : public Producer {
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
        /// <summary>
        /// Gets the class type info for the dynamic class instance type.
        /// </summary>
        virtual const ClassTypeInfo* GetClassTypeInfo() const { return TypeInfo; }
    public:
        /// <summary>
        /// Construct a new search producer.
        /// </summary>
        /// <param name="name">Name of producer.</param>
        /// <param name="scope">The application range of this producer.</param>
        /// <param name="resolutionModes">The phases in the configuration process when this producer can be invoked.</param>
        /// <param name="regexPattern">The pattern of Identifiers this producer matches against.</param>
        /// <param name="producerMode">The kinds of property access attempts to which this producer can respond.</param>
        SearchProducer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionModes, 
            IdentifierRegex& regexPattern, ProducerModesEnum producerMode);

        /// <summary>
        /// Creates a new property with the given identity in the specified container.
        /// </summary>
        /// <remarks>
        /// This implementation searches up the container hierarchy for a matching producer.
        /// </remarks>
        /// <param name="container">The container in which to create the new property.</param>
        /// <param name="identifier">The identity of the new property.
        /// This will at least specify a Name otherwise NULL is returned.
        /// If the type and position are not specified the producer must be able to determine these.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to create, or NULL if scope checking not required.</param>
        /// <returns>Returns a new property, or NULL if unable to create.</returns>
        virtual Property* Create(Container& container, Identifier& identifier, const Property* requester);

        /// <summary>
        /// Populates the container with new properties that match the specified regex identifier.
        /// </summary>
        /// <remarks>
        /// If a property already exists, a new one is not created.
        /// It is up to the Producer to call Container.Add as necessary.
        /// </remarks>
        /// <param name="container">The container in which to create the new properties.</param>
        /// <param name="regexIdentifier">A regex identifier for the new properties to create.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to create, or NULL if scope checking not required.</param>
        virtual void Populate(Container& container, IdentifierRegex& regexIdentifier, const Property* requester);

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
