#pragma once

namespace Plato {

    class Property;
    class Container;
    class Path;

    namespace SearchStrategies {
        /// <summary>
        /// The possible search strategies when looking for an object in the container hierarchy.
        /// </summary>
        enum Enum {
            /// <summary>
            /// Searches up the container hierarchy and the standard aliases.
            /// </summary>
            Public,
            /// <summary>
            /// Searches the current container and the standard aliases.
            /// </summary>
            Protected,
            /// <summary>
            /// Searches the current container only.
            /// </summary>
            Private
        };
    }
    typedef SearchStrategies::Enum SearchStrategiesEnum;

    /// <summary>
    /// Various methods for searching container hierarchy.
    /// </summary>
    class Search {
    public:
        /// <summary>
        /// Searches for property in the container hierarchy.
        /// If the propertyPath is a simple name, then the search strategy is followed.
        /// If the propertyPath is a path, then the object it points to is returned.
        /// </summary>
        /// <remarks>
        /// DEBUG: Dealing with multivalued paths. Could make this return an Enumerator.
        /// </remarks>
        /// <param name="propertyPath">The path of the property to find.</param>
        /// <param name="searchStrategy">The search strategy to follow.</param>
        /// <param name="currentContainer">The current container to start from.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns the property if found, otherwise NULL.</returns>
        static Property* FindProperty(Path& propertyPath, SearchStrategiesEnum searchStrategy, 
                                        Container* currentContainer, const Property* requester, PropertyModesEnum mode);
    };
}
