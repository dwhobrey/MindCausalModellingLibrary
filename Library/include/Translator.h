#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class GroupContainer;
    class Connector;

    /// <summary>
    /// Base class for geometry translators.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Translators are used to transform connections from one geometry to another via
    /// the OrderGroup method.
    /// This method is functionally similar to the GroupFilter.OrderGroup method.
    /// For example, the user may want to rotate or invert the order of the connections
    /// from one flow before they are connected to another flow.
    /// </para>
    /// <para>
    /// Translators are typically passed as a parameter to a Connector.
    /// The translator would be used in a throw away fashion: i.e.
    /// the connections are transformed and then connected, rather than linking in a
    /// transformation stage in the connection path - which would add an extra epoch.
    /// </para>
    /// </remarks>
    class Translator : public Property {
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
        /// General constructor for creating a new translator.
        /// </summary>
        /// <param name="creator">The container that created this translator.</param>
        /// <param name="parent">The hierarchical parent of this translator.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        Translator(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope);

        /// <summary>
        /// Default constructor.
        /// </summary>
        Translator();

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Translator();

        /// <summary>
        /// Re-order the elements in the group as necessary so that
        /// Container.GetOrderedEnumerator() returns the required order
        /// when the elements of the To and From groups are paired off.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This default implementation does nothing.
        /// </para>
        /// <para>
        /// This is used by Standard.Connector to order group elements.
        /// </para>
        /// </remarks>
        /// <param name="connector">The Connector being resolved.</param>
        /// <param name="toGroup">The destination group whose elements are to be ordered.</param>
        /// <param name="fromGroup">The source group whose elements are to be ordered.</param>
        virtual void OrderGroup(Connector& connector, GroupContainer& toGroup, GroupContainer& fromGroup);

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
