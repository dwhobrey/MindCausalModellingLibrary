#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Arguments;
    class Identifier;
    class Property;
    class Container;

    /// <summary>
    /// Constructors are factories for creating new properties.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Constructors are similar to Producers, except that the user
    /// of the constructor must call the factory method when required.
    /// </para>
    /// <para>
    /// They are typically used like an alias to override standard properties 
    /// such as Filters.
    /// </para>
    /// </remarks>
    class Constructor : public Property {
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
        /// The type of properties that this constructor produces.
        /// </summary>
        const ClassTypeInfo* PropertyClassTypeInfo;
        /// <summary>
        /// Optional arguments to use when creating property.
        /// These may be overridden by caller of Factory method.
        /// </summary>
        Arguments* PropertyArguments;

    public:
        /// <summary>
        /// Create a new constructor declaration that uses the ClassTypeInfo factory
        /// to create new properties.
        /// </summary>
        /// <param name="creator">The container that created this constructor.</param>
        /// <param name="parent">The hierarchical parent of this constructor.</param>
        /// <param name="constructorName">Local name for the constructor.</param>
        /// <param name="constructorScope">The visibility of the constructor.</param>
        /// <param name="propertyClassTypeInfo">The type of objects produced by this constructor.</param>
        /// <param name="arguments">Optional arguments to pass to property constructor.</param>
        Constructor(Container* creator, Container* parent, Identifier& constructorName, PropertyScopesEnum constructorScope, 
            const ClassTypeInfo& propertyClassTypeInfo, Arguments* arguments); 

        /// <summary>
        /// Allow derived classes chance to tidy up.
        /// </summary>
        virtual ~Constructor();

        /// <summary>
        /// The factory method for creating new properties.
        /// </summary>
        /// <param name="arguments">The arguments passed to the new property constructor.</param>
        /// <returns>Returns a new property, or NULL if unable to create one.</returns>
        virtual Property* Factory(Arguments* arguments);

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
