#pragma once

namespace Plato {

    class Property;
    class GroupContainer;
    class PropertyEnumerator;

    /// <summary>
    /// Implements an enumerator over the items in a Group Tree hierarchy.
    /// </summary>
    class GroupTreeEnumerator  {
#pragma region // Instance Variables.
        /// <summary>
        /// The top container over which to enumerate.
        /// </summary>
    protected:
        GroupContainer* mContainer;
        /// <summary>
        /// Holds state details at points where enumeration branches.
        /// </summary>
        vector<PropertyEnumerator*>* mEnumeratorList;
        /// <summary>
        /// Indicates whether the enumerator is reset, i.e. positioned before the first item.
        /// </summary>
        bool mIsReset;
        /// <summary>
        /// When true, only terminal GroupContainer's are returned by the enumerator.
        /// </summary>
        bool mIsTerminalGroup;
    
    public:
        /// <summary>
        /// Cache of the current item.
        /// </summary>
        Property* Current;
#pragma endregion

        /// <summary>
        /// Constructor for an enumerator over the items in a Group Tree hierarchy.
        /// </summary>
        /// <param name="container">The container to enumerate over.</param>
        /// <param name="isTerminalGroup">When true, only terminal GroupContainer's are returned by the enumerator.</param>
        GroupTreeEnumerator(GroupContainer& container, bool isTerminalGroup);

        /// <summary>
        /// Deallocate enumerator.
        /// </summary>
        ~GroupTreeEnumerator();

        /// <summary>
        /// Sets the enumerator to the start, ready for re-use.
        /// </summary>
        void Reset();

        /// <summary>
        /// Move to the next item, if any.
        /// </summary>
        /// <returns>Returns true if there are more items.</returns>
        bool MoveNext();

        /// <summary>
        /// Returns the next item or NULL if end of enumeration.
        /// </summary>
        /// <remarks>Calls MoveNext() and if true returns Current.</remarks>
        /// <returns>Returns the next item or NULL if end of enumeration.</returns>
        inline Property* Next() { return MoveNext() ? Current : NULL; }
    };
}
