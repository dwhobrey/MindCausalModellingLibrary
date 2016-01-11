#pragma once

namespace Plato {

    class Property;
    class Container;
    class GroupContainer;
    class PathNode;
    class PathNodeEnumerator;
    class Path;
    class Connector;

#pragma region // PathEnumeratorState.
    /// <summary>
    /// During enumeration a stack is maintained of details on each path node enumeration.
    /// </summary>
    class PathEnumeratorState {
    public:
        /// <summary>
        /// Tracks the current position in the path,
        /// NULL = reset position, otherwise a ref to a PathNode in Path.
        /// </summary>
        PathNode* CurrentPathNode;

        /// <summary>
        /// An enumerator over the items of a path node.
        /// </summary>
        PathNodeEnumerator* NodeEnumerator;

        /// <summary>
        /// The current container context for the path node.
        /// </summary>
        Container* CurrentContainer;

        /// <summary>
        /// The current position in the group tree associated with the path node.
        /// </summary>
        GroupContainer* PathGroupTree;

        /// <summary>
        /// During group retraversal, this is set to the number of further items required to complete the group.
        /// </summary>
        int CountRemaining;

        /// <summary>
        /// Indicates when PathNode chain should be deleted.
        /// </summary>
        bool DoDelete;

        /// <summary>
        /// Save path enumeration state details.
        /// Also sets up node enumerator.
        /// </summary>
        /// <param name="container">The current container to use for path node enumerator.</param>
        /// <param name="pathNode">The current path node, which is part of path.</param>
        /// <param name="skipEnumerator">If true, skips setting node enumerator.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <param name="pathGroupTree">The current position in the group tree.</param>
        /// <param name="countRemaining">The number of additional items needed to complete group.</param>
        /// <param name="doDelete">Indicates when PathNode chain should be deleted.</param>
        PathEnumeratorState(Container* container, PathNode* pathNode, bool skipEnumerator, 
            const Property* requester, PropertyModesEnum mode, GroupContainer* pathGroupTree, int countRemaining, bool doDelete);

        /// <summary>
        /// Deallocate resources.
        /// </summary>
        ~PathEnumeratorState();
    };
#pragma endregion

    /// <summary>
    /// Implements an enumerator over the items in a Path object's path.
    /// </summary>
    class PathEnumerator {
#pragma region // Instance Variables.
    public:
        /// <summary>
        /// The group tree for the path.
        /// </summary>
        GroupContainer* PathGroupTree;
        
        /// <summary>
        /// Path evaluation list: holds state details at points where evaluation branches.
        /// </summary>
        vector<PathEnumeratorState*>* PathEnumeratorStateList;

        /// <summary>
        /// The current item.
        /// </summary>
        Property* Current;

    protected:
        /// <summary>
        /// The path over which to enumerate.
        /// </summary>
        Path* mPath;

        /// <summary>
        /// Indicates whether the enumerator is reset, i.e. positioned before the first path item.
        /// </summary>
        bool mIsReset;

        /// <summary>
        /// The relative position in the container hierarchy to use with the path.
        /// </summary>
        Container* mPathCurrentContainer;

        /// <summary>
        /// The property requesting the enumeration - for scope checking.
        /// </summary>
        const Property* mRequester;

        /// <summary>
        /// The mode of access required.
        /// </summary>
        PropertyModesEnum mMode;

        /// <summary>
        /// When true, path evaluation proceeds in a width-first then depth order,
        /// otherwise it proceeds in a depth-first then width order.
        /// </summary>
        bool mIsWidthFirstDescent;

        /// <summary>
        /// A group handler for dealing with groupings.
        /// </summary>
        Connector* mGroupHandler;
#pragma endregion
    public:
        /// <summary>
        /// Constructor for an enumerator over the items of a path.
        /// </summary>
        /// <remarks>
        /// For Connector path evaluation, evaluation proceeds is a width-first order, which results in a large stack.
        /// Depth-first order is more efficient for general path enumeration.
        /// </remarks>
        /// <param name="path">The Path object over whose items to enumerate.</param>
        /// <param name="pathCurrentContainer">The relative position in the container hierarchy to use
        /// as the path's current container.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <param name="isWidthFirstDescent">When true, path evaluation proceeds in a width-first order.</param>
        /// <param name="groupHandler">An optional handler for dealing with groupings.</param>
        PathEnumerator(Path& path, Container* pathCurrentContainer, const Property* requester, 
            PropertyModesEnum mode, bool isWidthFirstDescent, Connector* groupHandler);

        /// <summary>
        /// Deallocate enumerator.
        /// </summary>
        ~PathEnumerator();
        
        /// <summary>
        /// Accessor for the current state of the enumerator.
        /// </summary>
        PathEnumeratorState* CurrentState();

        /// <summary>
        /// Sets the enumerator to the start, ready for re-use.
        /// </summary>
        void Reset();

        /// <summary>
        /// Move to the next item in the path, if any.
        /// </summary>
        /// <remarks>
        /// Think of the path as a regex pattern to apply over the container hierarchy.
        /// Example path pattern: /myregion/.*/input.
        /// Some possible paths to match against:
        /// a) /myregion/aregion/bregion/input/x/y
        /// b) /myregion/aregion/input
        /// c) /myregion/aregion/input/a
        /// d) /myregion/aregion/input/a/b
        /// e) /myregion/aregion/input/c/d/input
        /// Rules:
        /// 1) If a parent element has been returned as a leaf, then none of it child elements are returned.
        /// 2) This implies the shortest match paths are returned in preference to a possible longer match.
        /// 3) The Any PathNode is dealt with via the recursive substitution:
        /// A/.*/B ==> {A/B, A/.+/.*/B},
        /// e.g. /myregion/.*/input ==> a) /myregion/input, and b) /myregion/.+/.*/input.
        /// </remarks>
        /// <returns>Returns true if there are more items in the path.</returns>
        bool MoveNext();

        /// <summary>
        /// Returns the next item or NULL if end of enumeration.
        /// </summary>
        /// <remarks>Calls MoveNext() and then Current.</remarks>
        /// <returns>Returns the next item or NULL if end of enumeration.</returns>
        inline Property* Next() { return MoveNext() ? Current : NULL; }
    };
}
