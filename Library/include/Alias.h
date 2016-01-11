#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class Path;

    /// <summary>
    /// <para>
    /// An alias is used to declare an aribtrary object that may be referred to indirectly, i.e. proxied.
    /// It allows a container to declare and label objects that may be accessible (via paths) to other
    /// containers in the container hierarchy.
    /// </para>
    /// <para>
    /// Special treatment is given to the case when an Aliased object is one of the following types:
    /// <list type="number">
    /// <item><description>A Path object: the alias value is the object referred to by the path.</description></item>
    /// <item><description>An Property object: the alias value is the property's value.</description></item>
    /// </list>
    /// An alias can be used to declare or override default container types/properties, etc.
    /// </para>
    /// </summary>
    /// <remarks>Aliases cannot be proxied, but can be aliased.</remarks>
    class Alias : public Property {
#pragma region // Class methods & variables.
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
        /// <summary>
        /// Gets the class type info for the dynamic class instance type.
        /// </summary>
        virtual const ClassTypeInfo* GetClassTypeInfo() const { return TypeInfo; }
#pragma endregion
    private:
        /// <summary>
        /// The object being aliased.
        /// </summary>
        union {
            Property* mAliasedProperty;
            Path* mAliasedPath;
        };
        /// <summary>
        /// Flag indicating type of aliased object.
        /// </summary>
        bool IsPath;

        /// <summary>
        /// Flag indicating whether the alias owns the aliased object.
        /// If it does, the aliased object will also be deleted when the alias is.
        /// </summary>
        bool IsOwner;

        /// <summary>
        /// If the aliased object is a property, it's value may be cached.
        /// </summary>
        Property* mCachedProperty;

    public:
        /// <summary>
        /// Create a new alias declaration for a property.
        /// For example, a container might declare a new alias property that wraps some
        /// property that the container wants to expose for use by other containers.
        /// </summary>
        /// <param name="creator">The container that created this alias.</param>
        /// <param name="parent">The hierarchical parent of this alias.</param>
        /// <param name="aliasName">Local name for the alias.</param>
        /// <param name="aliasScope">The visibility of the alias.</param>
        /// <param name="aliasedProperty">The property being aliased.</param>
        /// <param name="isOwner">When true, the aliased object will be deleted when the alias is.</param>
        Alias(Container* creator, Container* parent, Identifier& aliasName, PropertyScopesEnum aliasScope, 
                Property* aliasedProperty=NULL, bool isOwner=true); 

        /// <summary>
        /// Create a new alias declaration for a path.
        /// For example, a container might declare a new alias property that wraps some
        /// path that the container wants to expose for use by other containers.
        /// </summary>
        /// <param name="creator">The container that created this alias.</param>
        /// <param name="parent">The hierarchical parent of this alias.</param>
        /// <param name="aliasName">Local name for the alias.</param>
        /// <param name="aliasScope">The visibility of the alias.</param>
        /// <param name="aliasedPath">The path being aliased. This is deleted when the alias is.</param>
        Alias(Container* creator, Container* parent, Identifier& aliasName, PropertyScopesEnum aliasScope,
                Path* aliasedPath); 

        /// <summary>
        /// Deallocate alias.
        /// </summary>
        virtual ~Alias();

        /// <summary>
        /// Override Property.GetProxy implementation.
        /// Get the aliased object rather than this.
        /// </summary>
        /// <remarks>
        /// Certain types of aliased object are treated specially:
        /// <list>
        /// <item><description>Path: value of alias is value of resolved path.</description></item>
        /// Because the alias might be part of a distributed container, the
        /// path value is recomputed each time.
        /// <item><description>Property: value of alias is value of property.</description></item>
        /// </list>
        /// </remarks>
        /// <returns>Returns the resolved object, if found, otherwise the NullProperty.</returns>
        virtual Property* GetProxy();

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
