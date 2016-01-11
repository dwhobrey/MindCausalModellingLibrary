#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Property;
    class Container;
    class Identifier;
    class Path;
    class Translator;
    class Distribution;
    class Generator;

    ///<summary>
    /// The standard distributor for distributing an element to specified elements, e.g. regions.
    ///</summary>
    ///<remarks>
    /// Rely on converters in destination elements to handle connection,
    /// i.e. call converters to add connections.
    /// The generator should determine the neighbourhood to consider over which to invoke converters.
    /// Need to support two kinds of distribution:
    /// <list type="number">
    /// <item><description>
    /// Distribute flow over regions.
    /// </description></item>
    /// <item><description>
    /// Distribution of flow elements over region elements, e.g. rotating, inverting.
    /// </description></item>
    /// </list>
    /// Need to:
    /// <list type="number">
    /// <item><description>
    /// Rules for connecting flows distributed to a region.
    /// </description></item>
    /// <item><description>
    /// Local definitions for connecting flow, neighbour determination etc.
    /// </description></item>
    /// <item><description>
    /// Local converters.
    /// </description></item>
    /// <item><description>
    /// Note: there will be a need to specify the neighbourhood to consider when invoking converters.
    /// </description></item>
    /// </list>
    /// Should phenomenon be distributable, e.g. colour quale across visual field? Yes.
    /// So define phenomenon (signs) like flows?
    /// <para>
    /// Use cases:
    /// <list type="number">
    /// <item><description>
    /// Map a flow element to a number of elements in various regions.
    /// </description></item>
    /// <item><description>
    /// Map elements of a flow to specific elements in a specific region.
    /// </description></item>
    /// <item><description>
    /// Repeat 2) using a different region root.
    /// </description></item>
    /// <item><description>
    /// Transform geometry prior to connecting flow to region.
    /// For example, if adding to a layer, may want to rotate or invert
    /// before connecting to it.
    /// </description></item>
    /// <item><description>
    /// Normally distribute elements over a uniform region.
    /// </description></item>
    /// </list>
    /// </para>
    /// </remarks>
    class Distributor : public Property {
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
#pragma endregion
    public:
        /// <summary>
        /// General constructor. Apply translator to distributed element geometry before connecting.
        /// </summary>
        /// <param name="creator">The container that created this distributor.</param>
        /// <param name="parent">The hierarchical parent of this distributor.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        /// <param name="generator">The generator to use to create instances of elements to distribute.</param>
        /// <param name="regionPath">The Elements to distribute over. Could contain a filter.</param>
        /// <param name="translator">The translator to use to translate the geometry.</param>
        /// <param name="distribution">The particular distribution to use when distributing the elements.</param>
        Distributor(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope, 
            Generator& generator, Path& regionPath, Translator& translator, Distribution& distribution); 

        /// <summary>
        /// Standard constructor. Apply translator to distributed element geometry before connecting.
        /// </summary>
        /// <param name="generator">The generator to use to create instances of elements to distribute.</param>
        /// <param name="regionPath">The Elements to distribute over. Could contain a filter.</param>
        /// <param name="translator">The translator to use to translate the geometry.</param>
        /// <param name="distribution">The particular distribution to use when distributing the elements.</param>
        Distributor(Generator& generator, Path& regionPath, Translator& translator, Distribution& distribution); 

        /// <summary>
        /// Construct a distributor over a path.
        /// </summary>
        /// <param name="generator">The generator for creating the elements at the distribution destination site.</param>
        /// <param name="regionPath">A path specifying the distribution destination sites.</param>
        Distributor(Generator& generator, Path& regionPath);

        /// <summary>
        /// Apply the distributor.
        /// </summary>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container for this distributor.</param>
        virtual void Apply(ResolutionModesEnum resolutionMode, Container& parent);

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
