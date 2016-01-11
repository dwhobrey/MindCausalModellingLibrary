#pragma once

namespace Plato {

    class Arguments;
    class Filter;
    class IdentifierRegex;
    class ClassTypeInfo;

    /// <summary>
    /// Additional arguments for Name Filters.
    /// </summary>
    class NameFilterArguments : public Arguments {
    public:
        /// <summary>
        /// Indicates whether the filter should include or exclude the items.
        /// When true, only the items that pass the filter are added to the
        /// filter's container. When false, the items that do not pass
        /// the filter are added to the filter's container.
        /// </summary>
        bool IncludeWhenMatches;
        /// <summary>
        /// The regex to use in name comparisons for match.
        /// </summary>
        string* ComparisonRegex;
        /// <summary>
        /// Additional Name filter arguments.
        /// </summary>
        /// <param name="comparisonRegex">The regex name string to match against when determining membership.</param>
        /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
        NameFilterArguments(string& comparisonRegex, bool includeWhenMatches);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        NameFilterArguments(const NameFilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~NameFilterArguments();
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };

    /// <summary>
    /// Selects the subcontainers in a subset of the container.
    /// Uses a Regex name to determine items that pass filter.
    /// </summary>
    class NameFilter : public Filter {
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
        /// <param name="arguments">The AdditionalArguments must be set to a NameFilterArguments object.</param>
        NameFilter(FilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~NameFilter();
        /// <summary>
        /// Factory for creating a new name filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a NameFilterArguments object.</param>
        static NameFilter* Factory(FilterArguments* arguments);
    
    protected:
        /// <summary>
        /// Filter the items based on regex name match.
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
