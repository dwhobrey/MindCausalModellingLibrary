#pragma once

namespace Plato {

    class Property;
    class IdentifierRegex;

    /// <summary>
    /// Implements a regex identifier checking enumerator for the items in an IEnumerator.
    /// </summary>
    class IdentifierEnumerator {    
    protected:
        /// <summary>
        /// A reference to the source properties to check the item identity of.
        /// </summary>
        vector<Property*> * mProperties;

        /// <summary>
        /// The current position in the property vector.
        /// </summary>
        int mPosition;

        /// <summary>
        /// The regex Identifier for the properties to fetch.
        /// </summary>
        const IdentifierRegex* mIdentifierRegex;

        /// <summary>
        /// Indicates whether the enumerator should include or exclude the items.
        /// </summary>
        bool mIncludeWhenMatches;

        /// <summary>
        /// The property requesting the enumeration - for identity checking.
        /// </summary>
        const Property* mRequester;

    public:
        /// <summary>
        /// The current property.
        /// </summary>
        Property* Current;
   
        /// <summary>
        /// Creates a regex identifier enumerator for properties.
        /// The items are filtered according to whether they match the regex identifier.
        /// </summary>
        /// <param name="properties">The properties to enumerate over.</param>
        /// <param name="regexIdentifier">The regex Identifier for the properties to fetch.</param>
        /// <param name="includeWhenMatches">Indicates whether to return items that match, or do not match.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to enumerate, or NULL if scope checking not required.</param>
        IdentifierEnumerator(vector<Property*> & properties, const IdentifierRegex& regexIdentifier, 
            bool includeWhenMatches, const Property* requester);
        /// <summary>
        /// Deallocate enumerator.
        /// </summary>
        ~IdentifierEnumerator();

        /// <summary>
        /// Returns the enumerator to the start ready for re-use.
        /// </summary>
        void Reset();

        /// <summary>
        /// Move to the next item that matches, if possible.
        /// </summary>
        /// <returns>Returns true if there are more items in the enumerator.</returns>
        bool MoveNext();

        /// <summary>
        /// Returns a pointer to the next property or NULL if end of enumeration.
        /// </summary>
        /// <returns>Returns the next property or NULL if end of enumeration.</returns>
        inline Property* Next() { return MoveNext() ? Current : NULL; }
    };
}
