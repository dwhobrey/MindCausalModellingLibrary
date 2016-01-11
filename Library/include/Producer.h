#pragma once

namespace Plato {

    class Property;
    class ClassTypeInfo;
    class Identifier;
    class IdentifierRegex;
    class Generator;
    class Container;

    /// <summary>
    /// Producers are used to add elements to a container on demand.
    /// </summary>
    /// <remarks>
    /// Producers are invoked in two circumstances:
    /// <list>
    /// <item>
    /// A producer is invoked when an attempt is made to access a non-existant named property
    /// in a container. The producer can either create the missing property or ignore the access attempt.
    /// </item>
    /// <item>
    /// When referred to in a path the producer is invoked to create the missing properties.
    /// </item>
    /// </list>
    /// </remarks>
    class Producer : public Property {
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
        /// The pattern of Identifiers this producer matches against.
        /// </summary>
        IdentifierRegex* RegexIdentifier;
        /// <summary>
        /// The generator for new elements.
        /// </summary>
        /// <remarks>This may be NULL if new elements are created by the producer.</remarks>
        Generator* ProducerGenerator;
        /// <summary>
        /// The phases in the configuration process when this producer can be invoked.
        /// </summary>
        ResolutionModesEnum ResolutionMode;
        /// <summary>
        /// The kinds of property access attempts to which this producer can respond.
        /// </summary>
        ProducerModesEnum ProducerMode;

        /// <summary>
        /// Construct a new producer.
        /// </summary>
        /// <param name="name">Name of producer.</param>
        /// <param name="scope">The application range of this producer.</param>
        /// <param name="resolutionModes">The phases in the configuration process when this producer can be invoked.</param>
        /// <param name="regexIdentifier">The pattern of Identifiers this producer matches against.</param>
        /// <param name="producerMode">The kinds of property access attempts to which this producer can respond.</param>
        /// <param name="generator">The generator to use when adding new elements, or NULL if not necessary.</param>
        Producer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionModes, 
            IdentifierRegex& regexIdentifier, ProducerModesEnum producerMode, Generator* generator);

        /// <summary>
        /// Allow derived classes chance to tidy up.
        /// </summary>
        virtual ~Producer();

        /// <summary>
        /// Creates a new property with the given identity in the specified container.
        /// </summary>
        /// <remarks>
        /// The implementation will include context information, such as:
        /// a) stage of configuration, b) required type, c) point distribution.
        /// A container may have a number of producers.
        /// It is up to the Producer to call Container.Add as necessary.
        /// The producer must copy the identifier.
        /// This default implementation calls the Generator and adds the result to the container.
        /// </remarks>
        /// <param name="container">The container in which to create the new property.</param>
        /// <param name="identifier">The identity of the new property.
        /// This will at least specify a Name otherwise NULL is returned.
        /// If the type and position are not specified the producer must be able to determine these.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to create, or NULL if scope checking not required.</param>
        /// <returns>Returns a new property, or NULL if unable to create.</returns>
        virtual Property* Create(Container& container, const Identifier& identifier, const Property* requester);

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
        virtual void Populate(Container& container, const IdentifierRegex& regexIdentifier, const Property* requester);

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
