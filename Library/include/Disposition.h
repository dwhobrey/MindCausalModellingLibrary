#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Container;
    class Path;
    class Bundle;

    /// <summary>
    /// Base class for modelling dispositions.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Dispositions (DIDs) are modelled as classes that take dynamic bundles of inputs and outputs.
    /// DIDs can be born or die, and can be superposed in regions.
    /// </para>
    /// <para>
    /// Dispositions (DIDs) are modelled as classes that take dynamic bundles of inputs and outputs.
    /// DIDs can be born or die, and can be superposed in regions.
    /// </para>
    /// <para>
    /// Permitted operations: AddBundle, AddConnection, AddConverter, AddAlias, AddLink, AddLocation.
    /// But not: AddFlow, AddRegion.
    /// </para>
    /// <para>
    /// The container holds Bundles for the categories of inputs and outputs.
    /// </para>
    /// </remarks>
    class Disposition : public Container {
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

    public:
        /// <summary>
        /// Indicates if this DID is superimposed in the region.
        /// </summary>
        bool IsSuperposed;

        /// <summary>
        /// Tells model that output doesn't vary if input remains same.
        /// </summary>
        bool IsInputSafe;

#pragma region // Constructors.
    public:
        /// <summary>
        /// Create a new disposition.
        /// </summary>
        /// <param name="creator">The element that created this element.</param>
        /// <param name="parent">The hierarchical parent of this element.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        Disposition(Container* creator = NULL, Container* parent = NULL,
            Identifier& elementName = Identifier::Generate("Disposition"), 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Disposition();
#pragma endregion

    public:
        /// <summary>
        /// Called by the simulator to update DIDs state.
        /// Equivalent to one epoch in simulated time.
        /// </summary>
        virtual void Update();

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
