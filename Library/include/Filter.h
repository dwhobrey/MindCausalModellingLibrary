#pragma once

namespace Plato {

    class Container;
    class Arguments;
    class Identifier;
    class Property;
    class ClassTypeInfo;

    /// <summary>
    /// Base class for filter arguments.
    /// </summary>
    /// <remarks>
    /// Note: rather than extending FilterArguments, derived classes should extent
    /// Arguments as necessary adding additional arguments, for example, see RegexFilterArguments.
    /// </remarks>
    class FilterArguments : public Arguments {
    public:
        /// <summary>
        /// The mode of access required.
        /// </summary>
        PropertyModesEnum Mode;

        /// <summary>
        /// The parent container.
        /// </summary>
        Container* Parent;

        /// <summary>
        /// The creator of the filter.
        /// </summary>
        Container* Creator;

        /// <summary>
        /// This points to the container on which to apply the filter operation.
        /// Normally this will be the Parent container, unless the filter is
        /// part of a chain. In the latter case, the Parent container's properties
        /// should be used as the context for the filter operation.
        /// </summary>
        /// <remarks>
        /// This will not always be the same as the Parent container.
        /// The Parent container's properties should be used
        /// for the context when applying the filter.
        /// </remarks>
        Container* Source;

        /// <summary>
        /// The property requesting the filter - for scope checking.
        /// </summary>
        const Property* Requester;

        /// <summary>
        /// Additional arguments to pass to derived class.
        /// </summary>
        Arguments* AdditionalArguments;

        /// <summary>
        /// Create a new set of filter arguments.
        /// </summary>
        /// <param name="creator">The container that created the filter.</param>
        /// <param name="parent">The hierarchical parent container of the filter.</param>
        /// <param name="source">The Source container to apply filter operation on.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to filter, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the filter.</param>
        /// <param name="additionalArguments">Additional arguments to pass to derived class.</param>
        FilterArguments(Container* creator, Container* parent, Container* source, 
            const Property* requester, PropertyModesEnum mode,Arguments* additionalArguments);
    
        /// <summary>
        /// Deallocate object.
        /// </summary>
        ~FilterArguments();
    };

    /// <summary>
    /// Base filter class.
    /// Filter the items in a container according to derived class criteria.
    /// Filters are never added to a container directly.
    /// Their type is added via a Constructor.
    /// The identifier for a filter is always the derived class's type name.
    /// Derived classes must implement the Factory method.
    /// </summary>
    /// <remarks>
    /// Filters are used to select a particular subset of items from a container.
    /// The subset should preserve the geometry of the original container unless transformed.
    /// The filter object acts as temporary lazy container to refer to containers
    /// during some operation, such as a transform.
    /// They allow the geometry of the containers to be changed etc.
    /// <para>
    /// The geometry shape and locational rules are used to specify the arrangement of the containers.
    /// </para>
    /// <para>
    /// For example, the user may refer to a container "mycontainer" with the path:
    /// <c>"a/b/c/mycontainer%myfilter",</c>
    /// where "myfilter" is the name of a filter to apply to the items of the container.
    /// </para>
    /// <para>
    /// The system provides some standard filters, such as:
    /// <list type="number">
    /// <item><description>
    /// IdentifierFilter,
    /// </description></item>
    /// <item><description>
    /// SearchFilter
    /// </description></item>
    /// </list>
    /// </para>
    /// <para>
    /// Users can define their own filters, give them a name and add them to a container, e.g.
    /// in the container code:
    ///  a) AddConstructor("myfilter1", MyFilter1.ClassTypeInfo); 
    ///  b) AddConstructor("myfilter2", MyFilter2.ClassTypeInfo, new MyFilter2Arguments(...));
    /// </para>
    /// </remarks>
    class Filter : public Container {
#pragma region // Class methods & variables.
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
        /// <summary>
        /// Signature of factory for creating a new filter instance via ClassTypeInfo.
        /// Derived filter classes must provide a factory method accessed via the class's ClassTypeInfo entry.
        /// </summary>
        /// <remarks>
        /// This default implementation returns NULL.
        /// </remarks>
        /// <param name="arguments">The arguments to the filter.</param>
        /// <returns>Returns the new filter if successful, otherwise NULL.</returns>
        static Filter* Factory(FilterArguments* arguments);
#pragma endregion

    public:
        /// <summary>
        /// Arguments for the base filter class.
        /// </summary>
        FilterArguments* Arguments;

    protected:
        /// <summary>
        /// Create a new filter.
        /// </summary>
        /// <param name="identifier">The identifier for this filter, normally the simple class name.</param>
        /// <param name="arguments">The filter's arguments.</param>
        Filter(Identifier& identifier, FilterArguments& arguments);
        /// <summary>
        /// Deallocate resources in derived classes too.
        /// </summary>
        virtual ~Filter();
    
    public:
        /// <summary>
        /// Appends a filter to this filter.
        /// The appended filter is applied first to the source container
        /// after which this filter is applied to the result.
        /// </summary>
        /// <remarks>
        /// More than one filter can be appended if necessary.
        /// DEBUG: Should this be deprecated: Filters are now inline with PathNodes?
        /// </remarks>
        /// <param name="filter">The filter to append to this filter.</param>
        void Append(Filter* filter);

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
