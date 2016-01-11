#pragma once

namespace Plato {

    class Arguments;
    class FilterArguments;
    class Filter;
    class ClassTypeInfo;
    class Point;

    /// <summary>
    /// Additional arguments for Point Filters.
    /// </summary>
    class PointFilterArguments : public Arguments {
    public:
        /// <summary>
        /// Indicates whether the filter should include or exclude the items.
        /// When true, only the items that pass the filter are added to the
        /// filter's container. When false, the items that do not pass
        /// the filter are added to the filter's container.
        /// </summary>
        bool IncludeWhenMatches;
        /// <summary>
        /// The regex to use in comparisons for match.
        /// </summary>
        PointRegex* RegexPoint;
        /// <summary>
        /// Additional Point filter arguments.
        /// </summary>
        /// <param name="regexPoint">The Point about which to test.</param>
        /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
        PointFilterArguments(PointRegex& regexPoint, bool includeWhenMatches);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        PointFilterArguments(const PointFilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~PointFilterArguments();
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };

    /// <summary>
    /// The PointFilter class is a filter that only passes containers at a particular point.
    /// </summary>
    /// <remarks>
    /// Note. Could extend this to include a neighbourhood, such as the layers and percentage as in the NeighbourhoodFilter.
    /// </remarks>
    class PointFilter : public Filter {
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
        /// Set up a new point filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a PointFilterArguments object.</param>
        PointFilter(FilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~PointFilter();
        /// <summary>
        /// Factory for creating a new point filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a PointFilterArguments object.</param>
        static PointFilter* Factory(FilterArguments* arguments);
        /// <summary>
        /// Filter the items based on point constraints.
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
