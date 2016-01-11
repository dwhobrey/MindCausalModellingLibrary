#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class Path;

    /// <summary>
    /// A link is a reference to another location in the Hierarchy, similar to UNIX symbolic links.
    /// </summary>
    class Link : public Container {
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
#pragma endregion
    private:
        /// <summary>
        /// The path to the destination of the link.
        /// </summary>
        const Path* mLinkDestination;
        /// <summary>
        /// The property value this link points to.
        /// </summary>
        Property* mDestinationValue;

    public:
        /// <summary>
        /// Create a new link.
        /// </summary>
        /// <param name="creator">The container that created this link.</param>
        /// <param name="parent">The hierarchical parent of this link.</param>
        /// <param name="linkName">Local name for the link.</param>
        /// <param name="linkScope">The visibility of the link.</param>
        /// <param name="linkDestination">The path to the destination of the link.</param>
        Link(Container* creator, Container* parent, Identifier& linkName, PropertyScopesEnum linkScope, 
                const Path& linkDestination);

        /// <summary>
        /// Deallocate link.
        /// </summary>
        virtual ~Link();

        /// <summary>
        /// Override Property.GetProxy implementation.
        /// Get the link destination object rather than this.
        /// </summary>
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
