#pragma once

namespace Plato {

    /// <summary>
    /// Additional arguments for Vacant Filters.
    /// </summary>
    class VacantFilterArguments : public Arguments {
    public:
        /// <summary>
        /// Additional Regex filter arguments.
        /// </summary>
        VacantFilterArguments();
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="arguments">The arguments to copy.</param>
        VacantFilterArguments(const VacantFilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~VacantFilterArguments();
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };

    /// <summary>
    /// The VacantFilter class is a filter for the points in container space not currently occupied.
    /// </summary>
    /// <remarks>
    /// Is this just {container space - mContainers}?
    /// </remarks>
    class VacantFilter : public Filter {
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
        /// <param name="arguments">The AdditionalArguments must be set to a VacantFilterArguments object.</param>
        VacantFilter(FilterArguments& arguments);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~VacantFilter();
        /// <summary>
        /// Factory for creating a new regex filter.
        /// </summary>
        /// <param name="arguments">The AdditionalArguments must be set to a VacantFilterArguments object.</param>
        static VacantFilter* Factory(FilterArguments* arguments);
    
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
