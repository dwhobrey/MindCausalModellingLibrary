#pragma once

namespace Plato {

    class Property;
    class Container;

    /// <summary>
    /// Implements a scope checking enumerator over the items in an IEnumerator.
    /// </summary>
    class ScopeEnumerator {
    public:
        /// <summary>
        /// Cache of the current path item.
        /// </summary>
        Property* Current;

    protected:
        /// <summary>
        /// A reference to the source enumerator to check the item scope of.
        /// </summary>
        Container* mSourceContainer;

        /// <summary>
        /// The property requesting the enumeration - for scope checking.
        /// </summary>
        const Property* mRequester;

        /// <summary>
        /// The next item position in container.
        /// </summary>
        vector<Property*>::iterator mNextPosition;

        /// <summary>
        /// The position just past last container item.
        /// </summary>
        vector<Property*>::iterator mEndPosition;

    public:
        /// <summary>
        /// Creates a scoped enumerator for an enumerator.
        /// The items are filtered according to whether they are in scope.
        /// </summary>
        /// <param name="container">The container to enumerate over.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        ScopeEnumerator(Container& container, const Property* requester);
        
        /// <summary>
        /// Deallocate enumerator.
        /// </summary>
        ~ScopeEnumerator();

        /// <summary>
        /// Returns the enumerator to the start ready for re-use.
        /// </summary>
        void Reset();

        /// <summary>
        /// Move to the next item that is in scope, if possible.
        /// </summary>
        /// <returns>Returns true if there are more items in the enumerator. </returns>
        bool MoveNext();
    };
}
