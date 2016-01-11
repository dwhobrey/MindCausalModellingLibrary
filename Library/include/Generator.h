#pragma once

namespace Plato {

    class Point;
    class Identifier;
    class Property;
    class Container;

    /// <summary>
    /// Provides a mechanism for a property to be replicated with
    /// variations that depend on the new element instance's identity etc.
    /// For example, if copies of a flow are to be distributed across a region,
    /// each copy may have to be tailored in some way, e.g. different connections
    /// or neighbourhood.
    /// </summary>
    /// <remarks>
    /// Generators are used to wrap the creation of properties during
    /// repeatative construction operations, i.e. in order to allow
    /// objects to be constructed that take arbitrary parameters.
    ///</remarks>
    class Generator {
    public:
        int Capacity; // The number of properties to generate, -1 means unbounded.

        /// <summary>
        /// Base constructor.
        /// </summary>
        /// <param name="capacity">The upper limit on the number of properties to be generated.
        /// -1 means no upper bound. </param>
        Generator(int capacity);

        /// <summary>
        /// Allow derived classes opportunity to clean up.
        /// </summary>
        virtual ~Generator();

        /// <summary>
        /// This method is called each time a new property is needed during the current
        /// generation operation.
        /// </summary>
        /// <remarks>Depending on the context, the identifier may or may not include
        /// a Name, Type, or Position term. The implmentation must check which terms
        /// need to be automatically set by it. The implementation must copy the identifier.</remarks>
        /// <param name="ownerContainer">The container in which it will reside.</param>
        /// <param name="identifier">The identity of the new property.
        /// Some of it's terms may need to be automatically set by the method.</param>
        /// <returns>Returns a new customised property.</returns>
        virtual Property* GetProperty(Container& ownerContainer, const Identifier& identifier);

        /// <summary>
        /// This method is called each time a new property is needed during the current
        /// generation operation.
        /// </summary>
        /// <remarks>The generator must be able to determine the Name and Type.</remarks>
        /// <param name="ownerContainer">The container in which it will reside.</param>
        /// <param name="position">The position of the new property.</param>
        /// <returns>Returns a new customised property.</returns>
        virtual Property* GetProperty(Container& ownerContainer, Point& position);
    };
}
