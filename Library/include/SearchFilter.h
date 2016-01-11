#pragma once

namespace Plato {

    class Arguments;
    class FilterArguments;
    class Filter;
    class ClassTypeInfo;
    class IdentifierRegex;

    /// <summary>
    /// Additional arguments for Search Filters.
    /// </summary>
    class SearchFilterArguments : public Arguments {
    public:
        /// <summary>
        /// Indicates whether the filter should include or exclude the items.
        /// When true, only the items that pass the filter are added to the
        /// filter's container. When false, the items that do not pass
        /// the filter are added to the filter's container.
        /// </summary>
        bool IncludeWhenMatches;
        /// <summary>
        /// The direction to search relative to current container.
        /// </summary>
        bool SearchUp;
        /// <summary>
        /// The regex identifier to match against in search.
        /// </summary>
        IdentifierRegex* RegexIdentifier;
        /// <summary>
        /// Additional Search filter arguments.
        /// </summary>
        /// <param name="regexIdentifier">The regex identifier to match against during search.</param>
        /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
        /// <param name="searchUp">The direction to search: up or down the container hierarchy.</param>
        SearchFilterArguments(IdentifierRegex& regexIdentifier, bool includeWhenMatches, bool searchUp);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        SearchFilterArguments(const SearchFilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~SearchFilterArguments();
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };
 
    /// <summary>
    /// Searches for the named containers.
    /// Uses a Regex to determine items that pass filter.
    /// </summary>
    class SearchFilter : public Filter {
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
        /// Set up a new search filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a SearchFilterArguments object.</param>
        SearchFilter(FilterArguments& arguments);

        /// <summary>
        /// Factory for creating a new regex filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a SearchFilterArguments object.</param>
        static SearchFilter* Factory(FilterArguments* arguments);

    protected:
        /// <summary>
        /// Filter the containers.
        /// </summary>
        virtual void RefreshContents();

        /// <summary>
        /// Recursively search up or down hierarchy for matching containers.
        /// Add matches to properties of this filter container.
        /// </summary>
        /// <remarks>DEBUG: Need to add scope checks.</remarks>
        /// <param name="searchContainer">Starting point for search.</param>
        void Search(Container* searchContainer);

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
