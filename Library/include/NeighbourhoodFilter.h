#pragma once

namespace Plato {

    class Arguments;
    class FilterArguments;
    class Filter;
    class ClassTypeInfo;

    /// <summary>
    /// Additional arguments for Neighbourhood Filters.
    /// </summary>
    class NeighbourhoodFilterArguments : public Arguments {
    public:
        /// <summary>
        /// When true, use NumberOfLayers, otherwise PercentageDistance.
        /// </summary>
        bool IsLayers;
        union {        
            /// <summary>
            /// The number of spherical layers of surrounding neighbours - e.g. like onion layers.
            /// </summary>
            int NumberOfLayers;
            /// <summary>
            /// Those neighbours within a percentage distance based on the container size.
            /// </summary>
            float PercentageDistance;
        };
        /// <summary>
        /// Additional Neighbourhood filter arguments for a spherical neighbourhood.
        /// </summary>
        /// <param name="numberOfLayers">The number of layers of neighbours to test.</param>
        NeighbourhoodFilterArguments(int numberOfLayers);
        /// <summary>
        /// Additional Neighbourhood filter arguments for a spherical neighbourhood.
        /// </summary>
        /// <param name="percentageDistance">Those neighbours within a percentage distance based on the container size.</param>
        NeighbourhoodFilterArguments(float percentageDistance);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        NeighbourhoodFilterArguments(const NeighbourhoodFilterArguments& arguments);
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };

    /// <summary>
    /// The NeighbourhoodFilter class returns the set of neighbouring containers,
    /// based on the constructor parameters.
    /// </summary>
    /// <remarks>
    /// <list type="number">
    /// <item><description>
    /// NumberOfLayers: spherical layers of surrounding neighbours - e.g. like onion layers.
    /// </description></item>
    /// <item><description>
    /// PercentageDistance: those neighbours within a percentage distance based on the container size.
    /// </description></item>
    /// </list>
    /// </remarks>
    class NeighbourhoodFilter : public Filter {
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
        /// <param name="arguments">The AdditionalArguments must be set to a NeighbourhoodFilterArguments object.</param>
        NeighbourhoodFilter(FilterArguments& arguments);
        /// <summary>
        /// Factory for creating a new regex filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a NeighbourhoodFilterArguments object.</param>
        static NeighbourhoodFilter* Factory(FilterArguments* arguments);
    
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
