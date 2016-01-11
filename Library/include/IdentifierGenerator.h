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
    /// A generator that constructs the property.
    /// The element must have a constructor that takes a single Identifier parameter.
    /// </summary>
    class IdentifierGenerator : public Generator {
    public:
        /// <summary>
        /// The name of the property to generate.
        /// </summary>
        Identifier* PropertyName;

        /// <summary>
        /// The class type of the property to generate.
        /// </summary>
        ClassTypeInfo* PropertyType;

        /// <summary>
        /// The geometry to give the generated property.
        /// </summary>
        Geometry* PropertyGeometry;

        /// <summary>
        /// The parameters to pass to the constructor.
        /// </summary>
        Arguments* ConstructorArguments;

        /// <summary>
        /// Creates a generator that calls a class constructor to generate new properties.
        /// </summary>
        /// <remarks>DEBUG: Also define a CG that allows additional params to be included after name in constructor.</remarks>
        /// <param name="propertyName">The name for the new property.</param>
        /// <param name="propertyType">The class of the property to generate.</param>
        /// <param name="propertyGeometry">The geometry for new GeometricElements, or NULL if not applicable.</param>
        /// <param name="constructorArguments">The arguments to the class constructor, or NULL.</param>
        IdentifierGenerator(Identifier& propertyName, ClassTypeInfo& propertyType, Geometry* propertyGeometry, Arguments* constructorArguments);

        /// <summary>
        /// Deallocate objecct.
        /// </summary>
        virtual ~IdentifierGenerator();

        /// <summary>
        /// Generates the next property for the specified position.
        /// </summary>
        /// <param name="ownerContainer">The container that owns the generated property.</param>
        /// <param name="identifier">The identity of the new property.
        /// Some of it's terms may need to be automatically set by the method.</param>
        /// <returns>Returns the generated property on success, otherwise NULL.</returns>
        virtual Property* GetProperty(Container& ownerContainer, const Identifier& identifier);
    };
}
