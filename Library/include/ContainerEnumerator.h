#pragma once

namespace Plato {

    class Property;
    class Container;

    /// <summary>
    /// Class for holding the context of an enumeration over a Container.
    /// </summary>
    class ContainerIterator {
    public:
        /// <summary>
        /// The current property.
        /// </summary>
        Property* Current;
        /// <summary>
        /// The vector being iterated over.
        /// </summary>
        vector<Property*>* Properties;
        /// <summary>
        /// Indicates whether forward or reverse iterator.
        /// </summary>
        bool IsForward;
        /// <summary>
        /// The position in enumeration.
        /// </summary>
        int Position;
        /// <summary>
        /// The number of elements to enumerate over.
        /// </summary>
        int Count;
        /// <summary>
        /// Construct a new enumeration context.
        /// </summary>
        /// <param name="properties">The properties to iterate over.</param>
        /// <param name="isReverse">The direction of iteration.</param>
        ContainerIterator(vector<Property*>* properties, bool isReverse=false) {
            Properties = properties;
            Count = (int)(properties->size());
            if(isReverse) {
                Position = Count;
                Current = NULL;
                IsForward = false;
            } else {
                Position = 0;
                Current = NULL;
                IsForward = true;
            }
        }

        /// <summary>
        /// Returns true if there are more items.
        /// </summary>
        inline bool MoveNext() {
            if(IsForward) {
                if(Position!=Count) {Current = (*Properties)[Position++]; return true;}
            } else {
                if(Position!=0) {Current = (*Properties)[--Position]; return true;}
            }
            return false;
        }
    };

    /// <summary>
    /// Implements an enumerator over the items in a Container hierarchy.
    /// </summary>
    class ContainerEnumerator {

#pragma region // Instance Variables.
    public:
        /// <summary>
        /// Returns a pointer to the current property, or NULL if not set yet.
        /// </summary>
        Property* Current;

    protected:
        /// <summary>
        /// The top container over which to enumerate.
        /// </summary>
        Container* mContainer;

        /// <summary>
        /// Indicates whether the enumerator is reset, i.e. positioned before the first item.
        /// </summary>
        bool mIsReset;

        /// <summary>
        /// The property requesting the enumeration - for scope checking.
        /// </summary>
        const Property* mRequester;

        /// <summary>
        /// Holds state details at points where enumeration branches.
        /// </summary>
        vector<ContainerIterator*>* mEnumeratorList;

        /// <summary>
        /// When true, enumeration proceeds in a width-first then depth order,
        /// otherwise it proceeds in a depth-first then width order.
        /// </summary>
        bool mIsWidthFirstDescent;
        /// <summary>
        /// When true, items are returned in reverse order.
        /// </summary>
        bool mIsReverse;
        /// <summary>
        /// When true, container nodes are included in the enumeration.
        /// </summary>
        bool mIncludeContainers;
        /// <summary>
        /// When true, non-container leaf nodes are included in the enumeration.
        /// </summary>
        bool mIncludeLeaves;
        /// <summary>
        /// When true, container-leaf nodes (i.e. empty containers) are included in the enumeration.
        /// </summary>
        bool mIncludeLeafContainers;
#pragma endregion

    public:
        /// <summary>
        /// Constructor for an enumerator over the items in a container hierarchy.
        /// </summary>
        /// <param name="container">The container to enumerate over.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to fetch, or NULL if scope checking not required.</param>
        /// <param name="isWidthFirstDescent">When true, enumeration proceeds in a width-first order.</param>
        /// <param name="includeContainers">When true, container nodes are included in the enumeration.</param>
        /// <param name="includeLeaves">When true, non-container leaf nodes are included in the enumeration.</param>
        /// <param name="includeLeafContainers">When true, container leaf nodes are included in the enumeration.</param>
        /// <param name="isReverse">When true, items are returned in reverse order.</param>
        ContainerEnumerator(Container* container, const Property* requester=NULL, 
            bool isWidthFirstDescent=false, bool includeContainers=true, 
            bool includeLeaves=true, bool includeLeafContainers=true, 
            bool isReverse=false);

        /// <summary>
        /// Deallocate enumerator.
        /// </summary>
        ~ContainerEnumerator();

        /// <summary>
        /// IEnumerator.Reset implementation.
        /// Sets the enumerator to the start, ready for re-use.
        /// </summary>
        void Reset();

        /// <summary>
        /// IEnumerator.MoveNext implementation.
        /// Move to the next item, if any.
        /// </summary>
        /// <returns>Returns true if there are more items.</returns>
        bool MoveNext();

        /// <summary>
        /// Returns a pointer to the next property or NULL if end of enumeration.
        /// </summary>
        /// <remarks>
        /// Calls MoveNext() and then Current.
        /// Do not use this method if the collection contains nulls.
        /// </remarks>
        /// <returns>Returns the next property or NULL if end of enumeration.</returns>
        inline Property* Next() { return MoveNext() ? Current : NULL; }
    };
}
