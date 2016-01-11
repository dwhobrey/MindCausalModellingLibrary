#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;

    /// <summary>
    /// Models a bundle of connections.
    /// </summary>
    /// <remarks>
    /// Bundles are used to group DID connections into categories.
    /// For example, a DID may categorise its inputs into named
    /// groups such as "control", "data", "inhibitory" etc.
    ///
    /// A bundle has a geometry and contains a list of pads.
    ///
    /// A bundle is either an input or output bundle.
    /// </remarks>
    class Bundle : public Container {
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

    protected:
        /// <summary>
        /// Indicates if this is an input or output bundle.
        /// </summary>
        bool mIsInput;

    public:
        /// <summary>
        /// Constructs a new Bundle.
        /// </summary>
        /// <param name="creator">The element that created this element.</param>
        /// <param name="parent">The hierarchical parent of this element.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        /// <param name="isInput">If true, this is an input bundle, otherwise it's an output bundle.</param>
        Bundle(Container* creator, Container& parent, 
            Identifier& elementName = Identifier::Generate("Bundle"), 
            PropertyScopesEnum scope = PropertyScopes::Public, 
            bool isInput=false);
        
        /// <summary>
        /// Deallocate object.
        /// </summary>
        ~Bundle();

    protected:
        /// <summary>
        /// Creates a new connection pad with the specified name.
        /// </summary>
        /// <param name="identifier">Identifier of pad to create.
        /// This will typically include a Name and Position, although the Type may not be set.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to fetch, or NULL if scope checking not required.</param>
        /// <returns>Returns the requested Pad property, or NULL if it could not be created.</returns>
        virtual Property* PropertyFactory(Identifier& identifier, const Property* requester);

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
