#pragma once

namespace Plato {

    class Arguments;
    class FilterArguments;
    class Filter;
    class ClassTypeInfo;

    /// <summary>
    /// Additional arguments for Type Filters.
    /// </summary>
    class TypeFilterArguments : public Arguments {
    public:
        /// <summary>
        /// Indicates whether the filter should include or exclude the items.
        /// When true, only the items that pass the filter are added to the
        /// filter's container. When false, the items that do not pass
        /// the filter are added to the filter's container.
        /// </summary>
        bool IncludeWhenMatches;
        /// <summary>
        /// The type to use in comparisons for match.
        /// </summary>
        const ClassTypeInfo* ComparisonType;
        /// <summary>
        /// Additional Type filter arguments.
        /// </summary>
        /// <param name="comparisonType">The type to match against when determining membership.</param>
        /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
        TypeFilterArguments(const ClassTypeInfo& comparisonType, bool includeWhenMatches);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        TypeFilterArguments(const TypeFilterArguments& arguments);
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };

    /// <summary>
    /// Selects the subcontainers in a subset of the container.
    /// Compares type to determine items that pass filter.
    /// </summary>
    class TypeFilter : public Filter {
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
        /// Set up a new type filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a TypeFilterArguments object.</param>
        TypeFilter(FilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~TypeFilter();
        /// <summary>
        /// Factory for creating a new type filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a TypeFilterArguments object.</param>
        static TypeFilter* Factory(FilterArguments* arguments);
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
