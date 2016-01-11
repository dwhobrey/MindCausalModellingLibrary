#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;

    /// <summary>
    /// Regions are used to model the (semi) fixed framework that supports flows
    /// and DIDs.
    /// Regions can contain sub regions, flows and DIDs.
    /// </summary>
    class Region : public Container {
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

#pragma region // Constructors.
    protected:
        /// <summary>
        /// Create a new region.
        /// </summary>
        /// <param name="creator">The element that created this element.</param>
        /// <param name="parent">The hierarchical parent of this element.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        Region(Container* creator = NULL, 
            Container* parent = NULL, 
            Identifier& elementName = Identifier::Generate("Region"), 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Create a new region.
        /// </summary>
        /// <param name="parent">The element that created this element and is the parent.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        Region(Container* parent, 
            Identifier& elementName = Identifier::Generate("Region"), 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Create a new region.
        /// </summary>
        /// <param name="parent">The element that created this element and is the parent.</param>
        /// <param name="elementName">The name of the element. Must not be NULL.
        /// A new Identifier is created to wrap name.</param>
        /// <param name="scope">The scope of the element.</param>
        Region(Container* parent, const char* elementName, 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Region();
#pragma endregion

#pragma region // Emulation methods.
        /// <summary>
        /// Called every epoch to update the state of the element.
        /// This may then call BirthDeath and Superposition methods?
        /// </summary>
        virtual void Update();

        /// <summary>
        /// Concerned with construction and destruction of elements.
        /// </summary>
        virtual void BirthDeath();

        /// <summary>
        /// Concerned with superposed elements and their interaction.
        /// </summary>
        virtual void Superposition();
#pragma endregion

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
