#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Container;

    /// <summary>
    /// Caters for the specification of phenomena,
    /// by allowing their underlying dispositions
    /// to be specified, which span many regions.
    /// It acts as a folder for flow specs.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The phenomenon class is used to allow the specification of dispositional
    /// processes that span many regions, e.g. signs and quale!
    /// Typically, the elements defined within it would be distributed to other regions.
    /// </para>
    /// <para>
    /// The kind of use-cases would be:
    /// 1) distributing flows
    /// 2) specifying connections between distributed flows and other regions.
    /// 3) specifying connections between other regions.
    /// </para>
    /// <para>
    /// Permitted operations:
    /// AddDistributor, AddConnection, AddConverter, AddRestrictor,
    /// AddAlias, AddLink, AddLocation: in order to refer to Phenomenon collectively.
    /// But not: AddFlow, AddDisposition.
    /// </para>
    /// </remarks>
    class Phenomenon : public Container {
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
        /// Create a new phenomenon.
        /// </summary>
        /// <param name="creator">The element that created this element.</param>
        /// <param name="parent">The hierarchical parent of this element.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        Phenomenon(Container* creator = NULL, 
            Container* parent = NULL,
            Identifier& elementName = Identifier::Generate("Phenomenon"), 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Create a new phenomenon.
        /// </summary>
        /// <param name="parent">The element that created this element and is the parent.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        Phenomenon(Container* parent, 
            Identifier& elementName = Identifier::Generate("Phenomenon"), 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Create a new phenomenon.
        /// </summary>
        /// <param name="parent">The element that created this element and is the parent.</param>
        /// <param name="elementName">The name of the element. Must not be NULL.
        /// A new Identifier is created to wrap name.</param>
        /// <param name="scope">The scope of the element.</param>
        Phenomenon(Container* parent, const char* elementName, 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Phenomenon();
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
