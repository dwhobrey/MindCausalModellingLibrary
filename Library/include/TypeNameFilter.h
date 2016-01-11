#pragma once

namespace Plato {

    class Arguments;
    class FilterArguments;
    class Filter;
    class ClassTypeInfo;

    /// <summary>
    /// Additional arguments for TypeName Filters.
    /// </summary>
    class TypeNameFilterArguments : public Arguments {
    public:
        /// <summary>
        /// Indicates whether the filter should include or exclude the items.
        /// When true, only the items that pass the filter are added to the
        /// filter's container. When false, the items that do not pass
        /// the filter are added to the filter's container.
        /// </summary>
        bool IncludeWhenMatches;
        /// <summary>
        /// The regex type name to use in comparisons for match.
        /// </summary>
        const string* ComparisonRegex;
        /// <summary>
        /// Additional TypeName filter arguments.
        /// </summary>
        /// <param name="comparisonRegex">The regex type name string to match against when determining membership.</param>
        /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
        TypeNameFilterArguments(const string& comparisonRegex, bool includeWhenMatches);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        TypeNameFilterArguments(const TypeNameFilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~TypeNameFilterArguments();
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };

    /// <summary>
    /// Selects the subcontainers in a subset of the container.
    /// Uses a regex type name to determine items that pass filter.
    /// </summary>
    class TypeNameFilter : public Filter {
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
        /// Set up a new regex filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a TypeNameFilterArguments object.</param>
        TypeNameFilter(FilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~TypeNameFilter();
        /// <summary>
        /// Factory for creating a new type name regex filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a TypeNameFilterArguments object.</param>
        static TypeNameFilter* Factory(FilterArguments* arguments);
    protected:
        /// <summary>
        /// Filter the items based on regex match.
        /// </summary>
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
