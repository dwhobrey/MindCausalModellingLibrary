#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;

    /// <summary>
    /// Base class for pads.
    /// Pads model the actual point (DID) at which connections are connected.
    /// </summary>
    class Pad : public Property {
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
        /// Construct a new Pad.
        /// </summary>
        /// <param name="creator">The element that created this pad.</param>
        /// <param name="parent">The hierarchical parent of this pad.</param>
        /// <param name="identity">The name to assign to the new pad.</param>
        /// <param name="scope">The visibility of the pad.</param>
        Pad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Pad();

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
