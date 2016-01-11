#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class IdentifierRegex;
    class Property;
    class Container;
    class Restrictor;
    class Path;

    /// <summary>
    /// A Path restrictor that limits operations to those elements matching the path pattern.
    /// </summary>
    class PathRestrictor : public Restrictor {
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
        /// The restriction path to match against.
        /// </summary>
        Path* RestrictionPath;

        /// <summary>
        /// Constructs a path restrictor based on a Path path.
        /// </summary>
        /// <param name="operatorRegexTypeName">The type name of the operator this restrictor applies to.</param>
        /// <param name="includeWhenMatches">Indicates whether to allow items that pass filter,
        /// or deny items that pass filter.</param>
        /// <param name="scope">The scope of the operator to which this restrictor applies.</param>
        /// <param name="path">The path to filter on.</param>
        PathRestrictor(IdentifierRegex& operatorRegexTypeName, bool includeWhenMatches, PropertyScopesEnum scope, Path& path);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~PathRestrictor();

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
