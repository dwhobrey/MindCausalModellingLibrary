#pragma once

namespace Plato {

    class Container;
    class Property;
    class Identifier;
    class IdentifierRegex;

    /// <summary>
    /// ProducerListeners listen for production events and
    /// invoke an appropriate Producer.
    /// </summary>
    /// <remarks>
    /// ProducerListeners are normally added to a container
    /// and route production requests to the container's Producers.
    /// </remarks>
    /// <remarks>
    /// The container associated with a producer listener is specified when
    /// the producer listener object is added to the container.
    /// </remarks>
    class ProducerListener {
    private:
        /// <summary>
        /// The container associated with this listener.
        /// </summary>
        Container* mContainer;

    public:
        /// <summary>
        /// Constructs a new producer listener for the specified container.
        /// </summary>
        /// <param name="container">The container to add the producer listener to.</param>
        ProducerListener(Container& container);

        /// <summary>
        /// Creates a new property with the given identity in the specified container.
        /// </summary>
        /// <remarks>
        /// The implementation will include context information, such as:
        /// a) stage of configuration, b) required type, c) point distribution.
        /// A container may have a number of producers.
        /// It is up to the implementation to determine whether property Configuration is required
        /// and whether the container's producers need to be called.
        /// </remarks>
        /// <param name="identifier">The identity of the new property.
        /// This will at least specify a Name otherwise NULL is returned.
        /// If the type and position are not specified the producer must be able to determine these.
        /// The Create method must take a copy of the identifier.
        /// </param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to create, or NULL if scope checking not required.</param>
        /// <returns>Returns a new property, or NULL if unable to create.</returns>
        Property* Create(const Identifier& identifier, const Property* requester);

        /// <summary>
        /// Populates the container with new properties that match the specified regex identifier.
        /// </summary>
        /// <remarks>
        /// If a property already exists, a new one is not created.
        /// </remarks>
        /// <param name="regexIdentifier">A regex identifier for the new properties to create.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to create, or NULL if scope checking not required.</param>
        void Populate(const IdentifierRegex& regexIdentifier, const Property* requester);
    };
}
