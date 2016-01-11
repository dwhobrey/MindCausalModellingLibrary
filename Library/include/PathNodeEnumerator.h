#pragma once

namespace Plato {

    class Property;
    class Container;
    class GroupFilter;
    class PathNode;
    class ScopeEnumerator;

    /// <summary>
    /// Implements an enumerator over the items in a PathNode object, which may include filters.
    /// </summary>
    /// <remarks>
    /// Note, a PathNodeEnumrator does not descend the PathNode chain.
    /// That is done by PathEnumerator.
    /// </remarks>
    class PathNodeEnumerator {
    public:
        /// <summary>
        /// Cache of the current path item.
        /// </summary>
        Property* Current;
        /// <summary>
        /// This will be set to the instance of a GroupFilter if one is encountered.
        /// </summary>
        GroupFilter* NodeGroupFilter;

    protected:
        /// <summary>
        /// Indicates whether the enumerator is reset, i.e. positioned before the first path node item.
        /// </summary>
        bool mIsReset;

        /// <summary>
        /// The path node over which to enumerate.
        /// </summary>
        PathNode* mPathNode;

        /// <summary>
        /// The relative position in the container hierarchy to use with the PathNode.
        /// </summary>
        Container* mNodeCurrentContainer;

        /// <summary>
        /// The property requesting the enumeration - for scope checking.
        /// </summary>
        const Property* mRequester;

        /// <summary>
        /// The mode of access required.
        /// </summary>
        PropertyModesEnum mMode;

        /// <summary>
        /// The container to enumerate over.
        /// </summary>
        Container* mWorkingContainer;

        /// <summary>
        /// Reference to a temporary container created by a filter.
        /// </summary>
        Container* mFilterContainer;

        /// <summary>
        /// Reference to the resultant internal enumerator object for this node.
        /// </summary>
        ScopeEnumerator* mNodeEnumerator;

    public:
        /// <summary>
        /// Internal class used by Path to implement an enumerator over the items of a path node.
        /// </summary>
        /// <param name="currentPathNode">The current PathNode.</param>
        /// <param name="nodeCurrentContainer">The relative position in the container hierarchy to use
        /// with the path node.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        PathNodeEnumerator(PathNode& currentPathNode, Container* nodeCurrentContainer, const Property* requester, PropertyModesEnum mode);

        /// <summary>
        /// Deallocates enumerator.
        /// </summary>
        ~PathNodeEnumerator();

        /// <summary>
        /// Returns the enumerator to the start ready for re-use.
        /// </summary>
        void Reset();

        /// <summary>
        /// Move to the next item in the path node if possible.
        /// </summary>
        /// <returns>Returns true if there are more items in the path node. </returns>
        bool MoveNext();
    };
}
