#pragma once

namespace Plato {

    class Filter;
    class ClassTypeInfo;
    class FilterArguments;

    /// <summary>
    /// Base class for group filters.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Group filters are specified like normal filters in paths except that they are applied
    /// to the items in a path group.
    /// </para>
    /// <para>
    /// Currently group filter specification is limited to it being the last filter in a chain of filters.
    /// </para>
    /// </remarks>
    class GroupFilter : public Filter {
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
        /// Set up a new group filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a FilterArguments object.</param>
        GroupFilter(FilterArguments& arguments);

    protected:
        /// <summary>
        /// For GroupFilters, the RefreshContents method is called when the branch of the group tree referred to
        /// by the Source container needs to be ordered.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This implementation links the Source container properties to this container
        /// and then invokes the OrderGroup method.
        /// </para>
        /// </remarks>
        virtual void RefreshContents();

        /// <summary>
        /// Re-order the elements in the group as necessary so that
        /// Container.OrderedProperties returns the required order.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This default implementation does nothing.
        /// </para>
        /// <para>
        /// This is used by Standard.Connector to order elements.
        /// </para>
        /// <para>
        /// For GroupFilters the Source container and underlying filter container are the same
        /// due to the link made when the Apply method was called.
        /// </para>
        /// <para>The Source container always refers to a branch in a Connector group tree.</para>
        /// <para>
        /// Operations should be performed on the Source container if they are to have any effect,
        /// i.e. GroupFilter's do not return a new container.
        /// </para>
        /// </remarks>
        virtual void OrderGroup();

        /// <summary>
        /// Factory for creating a new filter instance via ClassTypeInfo.
        /// </summary>
        /// <param name="arguments">The arguments to the filter.</param>
        /// <returns>Returns the new filter if successful, otherwise NULL.</returns>
        static Filter* Factory(FilterArguments* arguments);

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
