#pragma once

namespace Plato {

    class Property;
    class Container;
    class Path;
    class PathNode;

    /// <summary>
    /// A helper class for performing regex matches on paths.
    /// </summary>
    class PathRegex {

    private:
        /// <summary>
        /// Internal helper class for CompareToRegex.
        /// Used for stacking the partial state of a regex comparison.
        /// </summary>
        class CompareToRegexState {
        public:
            /// <summary>
            /// The current path to compare with regex path.
            /// </summary>
            Path* mThisPath;
            /// <summary>
            /// The regex path to compare with main path.
            /// </summary>
            Path* mRegexPath;
            /// <summary>
            /// The current ThisPath position in the container hierarchy.
            /// </summary>
            Container* mThisPosition;
            /// <summary>
            /// The current RegexPath position in the container hierarchy.
            /// </summary>
            Container* mRegexPosition;

            /// <summary>
            /// General constructor.
            /// </summary>
            /// <param name="thisPath">The current path to compare with regex path.</param>
            /// <param name="regexPath">The regex path to compare with main path.</param>
            /// <param name="thisPosition">The current ThisPath position in the container hierarchy.</param>
            /// <param name="regexPosition">The current RegexPath position in the container hierarchy.</param>
            CompareToRegexState(Path* thisPath, Path* regexPath, Container* thisPosition, Container* regexPosition);
        };

    public:
        /// <summary>
        /// Compares the path of a property for a regex match against the partial regex path.
        /// </summary>
        /// <remarks>
        /// This implementation currently simply calls the CompareToRegex method with the property's path.
        /// A custom implementation would be more efficient.
        /// </remarks>
        /// <param name="prop">A property whose path is compared for a match against the regex path.</param>
        /// <param name="regexPath">The partial path to test the property path for a match against.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns true if the paths match according to the specified parameters.</returns>
        static bool CompareToRegex(Property* prop, Path* regexPath, const Property* requester, PropertyModesEnum mode);

        /// <summary>
        /// Compares a path for a regex match against the partial regex path.
        /// </summary>
        /// <remarks>
        /// Assumption: since cannot easily compare two complex regex patterns,
        /// the fixed path is assumed not to contain complex regex patterns.
        /// The match can be performed in the following ways:
        /// a) From tail up until regexPath exhausted.
        /// b) From head down until regexPath exhausted.
        /// c) Complete full match using '^' and '$' nodes.
        /// The starting point can be specified via position, i.e. new head or tail.
        ///
        /// Paths = "/AAA/BBB/CCC/DDD", "AAA/BBB/CCC/DDD/EEE/FFF".
        /// RegexPaths = "DDD", "D"+WildPattern, "./D"+WildPattern,
        ///             AnyPattern / "D" + WildPattern, etc.
        /// Multiple AnyPattern expansion is supported:  A/.*/B ==> {A/B, A/.+/.*/B}.
        /// </remarks>
        /// <param name="fixedPath">An actual path to compare for a match against the regex path.</param>
        /// <param name="regexPath">The partial path to test the fixed path for a match against.</param>
        /// <param name="currentContainer">The current container to use for path comparison when necessary.
        /// This is the container for the position node.</param>
        /// <param name="position">The starting point in the fixed path, when NULL the default position is the Head.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns true if the paths match according to the specified parameters.</returns>
        static bool CompareToRegex(Path* fixedPath, Path* regexPath, Container* currentContainer, 
            PathNode* position, const Property* requester, PropertyModesEnum mode);

    private:
        /// <summary>
        /// Helper method for CompareToRegex.
        /// Compares two paths in normal form for a match.
        /// </summary>
        /// <param name="q">The q path.</param>
        /// <param name="r">The r path.</param>
        /// <returns>Returns true if paths match.</returns>
        static bool ComparePaths(Path* q, Path* r);

        /// <summary>
        /// Helper method for CompareToRegex.
        /// If path starts with an Any node, stack a Wild/Any version and cut Any from path.
        /// </summary>
        /// <param name="pAnyNode">A reference to the head Any node, or NULL if not an Any node.</param>
        /// <param name="q">The main path.</param>
        /// <param name="r">The regex path.</param>
        /// <param name="qCurrent">The position for the main path.</param>
        /// <param name="rCurrent">The position for the regex path.</param>
        /// <param name="isRegex">True if checking the regex path, otherwise main path is checked.</param>
        /// <param name="stateStack">The stack onto which to put the Wild/Any path if necessary.</param>
        /// <returns>Returns true if this was an Any node path.</returns>
        static bool StackAny(PathNode* pAnyNode, Path* q, Path* r, Container* qCurrent, Container* rCurrent, 
            bool isRegex, stack<CompareToRegexState*> & stateStack);

        /// <summary>
        /// Helper method for CompareToRegex.
        /// Cuts matching path nodes from the two paths q and r until an Any node is encountered.
        /// </summary>
        /// <param name="qCurrent">The context for the q path.</param>
        /// <param name="q">The q path.</param>
        /// <param name="rCurrent">The context for the r path.</param>
        /// <param name="r">The r path.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <returns>Returns false if the paths are found not to match.
        /// True means paths may match, or contain an Any node, or be of different lengths.</returns>
        static bool CutMatchingNodes(Container* & qCurrent, Path* & q, Container* & rCurrent, Path* & r, 
            const Property* requester);

        /// <summary>
        /// Helper method for CompareToRegex.
        /// Loop over nodes until end or Any.
        /// Evaluate any non-normal nodes encountered.
        /// </summary>
        /// <param name="currentContainer">The container to use as the context.
        /// Returns the new value for the current container, which may be NULL.</param>
        /// <param name="path">The Path to expand. Returns a new path with the nodes evaluated to a chain of normal
        /// nodes down to the first Any or end.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <returns>Returns the Any node if encountered, or NULL if end reached.</returns>
        static PathNode* ExpandPath(Container* & currentContainer, Path* & path, const Property* requester);

        /// <summary>
        /// Helper method for CompareToRegex.
        /// Evaluates the prefix or origin of a PathNode returning a path that
        /// is the expanded prefix suffixed by any following PathNodes.
        /// </summary>
        /// <param name="currentContainer">The container to use as the context.
        /// Returns the new value for the current container, which may be NULL.</param>
        /// <param name="node">The PathNode to expand. Returns the next node.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <returns>Returns a new path with the prefix expanded to a chain of normal
        /// nodes followed by the rest of the PathNode chain of the node.</returns>
        static Path* ExpandNode(Container* & currentContainer, PathNode* & node, const Property* requester);

        /// <summary>
        /// Helper method for CompareToRegex.
        /// Evaluates paths containing Search prefixes
        /// and returns the resulting path.
        /// </summary>
        /// <param name="currentContainer">The container to use as the context. Resets this to NULL if search occurred.</param>
        /// <param name="path">The path to check for Search prefixes. Returns a path without Search prefixes.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        static void PerformSearch(Container* & currentContainer, Path* & path, 
            const Property* requester, PropertyModesEnum mode);
    };
}
