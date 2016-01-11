#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Arguments;
    class Geometry;
    class Point;
    class Identifier;
    class Property;
    class Container;

    /// <summary>
    /// A generator that simply returns the same property regardless of position etc.
    /// </summary>
    class IdentityGenerator : public Generator {
    public:
        /// <summary>
        /// The property to return.
        /// </summary>
        Property* IdentityProperty;

        /// <summary>
        /// Constructs a new IdentityGenerator for the given property.
        /// </summary>
        /// <param name="identityProperty">The property to return through generation.</param>
        IdentityGenerator(Property* identityProperty);

        /// <summary>
        /// Getter for the generated property.
        /// </summary>
        /// <param name="ownerContainer">The property that owns the generated property.</param>
        /// <param name="identifier">The identity of the new property.
        /// Some of it's terms may need to be automatically set by the method.</param>
        /// <returns>Returns a property for the given position.</returns>
        virtual Property* GetProperty(Container& ownerContainer, const Identifier& identifier);
    };
}
