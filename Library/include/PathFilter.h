#pragma once

namespace Plato {

    class Arguments;
    class FilterArguments;
    class Filter;
    class ClassTypeInfo;
    class Path;

    /// <summary>
    /// Additional arguments for Path Filters.
    /// </summary>
    class PathFilterArguments : public Arguments {
    public:
        /// <summary>
        /// Indicates whether the filter should include or exclude the items.
        /// When true, only the items that pass the filter are added to the
        /// filter's container. When false, the items that do not pass
        /// the filter are added to the filter's container.
        /// </summary>
        bool IncludeWhenMatches;
        /// <summary>
        /// The path to match a container's path against.
        /// </summary>
        Path* RegexPath;
        /// <summary>
        /// Additional Path filter arguments.
        /// </summary>
        /// <param name="regexPath">The regex filter path to match an element's path against.</param>
        /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
        PathFilterArguments(Path& regexPath, bool includeWhenMatches);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        PathFilterArguments(const PathFilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~PathFilterArguments();
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };

    /// <summary>
    /// This filter tests the path of a container for a match.
    /// Note: this implies a Filter container may contain elements from
    /// different sources.
    /// </summary>
    class PathFilter : Filter {
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
        /// Set up a new path filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a RegexFilterArguments object.</param>
        PathFilter(FilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~PathFilter();
        /// <summary>
        /// Factory for creating a new regex filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a PathFilterArguments object.</param>
        static PathFilter* Factory(FilterArguments* arguments);
        /// <summary>
        /// Filter the items based on path match.
        /// </summary>
    protected:
        virtual void RefreshContents();

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
