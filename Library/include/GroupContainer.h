#pragma once

namespace Plato {

    class Container;
    class ClassTypeInfo;
    class Property;
    class GroupContainer;
    class PathNode;

    /// <summary>
    /// When a path is enumerated that contains groups,
    /// a group hierarchy is constructed out of GroupContainer nodes.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The GroupContainer class is derived from Container so that it can contain
    /// sub group properties. It acts as a proxy for a specified property and allows
    /// sub group properties to be added as container members.
    /// </para>
    /// <para>
    /// Note, although the value of the container is set to the original property,
    /// GroupContainer additions get added to this container rather than the property.
    /// This relies on Container.ContainerProperties not going through Value.
    /// </para>
    /// </remarks>
    class GroupContainer : public Container {
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

    public:
        /// <summary>
        /// Flag indicating when the group is complete,
        /// i.e. contains the required number of elements,
        /// and the elements are in the required order.
        /// </summary>
        bool IsComplete;

        /// <summary>
        /// Flag indicating whether this group was formed
        /// from an unbounded expression.
        /// </summary>
        bool IsUnbounded;

        /// <summary>
        /// The group number of this property in the group tree.
        /// </summary>
        int GroupNumber;

        /// <summary>
        /// The property that this container mimics.
        /// </summary>
        Property* mValue;

        /// <summary>
        /// Constructs a new GroupContainer container.
        /// </summary>
        /// <param name="prop">The property that this GroupContainer mimics.</param>
        /// <param name="parentGroup">The group tree parent group.</param>
        /// <param name="groupNumber">The group number of this property.</param>
        GroupContainer(Property* prop, GroupContainer* parentGroup, int groupNumber);

        /// <summary>
        /// Deallocate group tree.
        /// </summary>
        virtual ~GroupContainer();

        /// <summary>
        /// Override Property.GetProxy implementation.
        /// Get the mimicked property rather than this.
        /// </summary>
        /// <returns>Returns the mimiced property.</returns>
        virtual Property* GetProxy();

        /// <summary>
        /// Construct a new tree for the property using the pathNode chain to initialise the tree.
        /// </summary>
        /// <remarks>
        /// This constructs an initial tree as a sequence of subgroup nodes leading to the property.</remarks>
        /// <param name="prop">A subgroup property within the tree.</param>
        /// <param name="pathNode">The PathNode for this property.
        ///  The parent PathNodes are used to determine if there are any preceeding groups in the tree.</param>
        /// <param name="treeRoot">Upon return this refers to the root of the new tree.</param>
        /// <returns>Returns the position of the property in the tree.</returns>
        static GroupContainer* Factory(Property* prop, PathNode* pathNode, GroupContainer* & treeRoot);

        /// <summary>
        /// Adds a sub group to the current group.
        /// </summary>
        /// <param name="prop">The property associated with the group to add.</param>
        /// <param name="groupNumber">The group number of the property.</param>
        /// <returns>Returns the sub GroupContainer node of the new group.</returns>
        GroupContainer* AddGroup(Property* prop, int groupNumber);

        /// <summary>
        /// Adds a property to the current group.
        /// </summary>
        /// <param name="prop">The property to add.</param>
        void AddProperty(Property* prop);

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
