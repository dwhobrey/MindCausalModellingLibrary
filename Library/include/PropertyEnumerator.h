#pragma once

namespace Plato {

    class Property;

    /// <summary>
    /// An enumerator over a vector of Properties.
    /// </summary>
    /// <remarks>
    /// <para>
    /// This enumerator is for when the context of an iteration needs to be saved.
    /// </para>
    /// <para>
    /// This enumerator allows appending to container without invalidating enumeration.
    /// </para>
    /// </remarks>
    class PropertyEnumerator {
    public:
        /// <summary>
        /// A reference to the vector being iterated over.
        /// </summary>
        vector<Property*>* Properties;
        /// <summary>
        /// The current property in enumeration.
        /// </summary>
        Property* Current;
    protected:
        /// <summary>
        /// The current index position.
        /// </summary>
        int mPosition;
    public:
        /// <summary>
        /// Construct a new enumeration context.
        /// </summary>
        /// <param name="properties">The vector to be enumerated over.</param>
        inline PropertyEnumerator(vector<Property*> & properties) {
            Properties = &properties;
            Current = NULL;
            mPosition = -1;
        }
        /// <summary>
        /// Reset the enumerator.
        /// </summary>
        inline void Reset() { mPosition = -1; }
        /// <summary>
        /// Check if there are more items.
        /// </summary>
        /// <returns>Returns true if there are more items.</returns>
        inline bool MoveNext() {
            if(++mPosition<(int)Properties->size()) { Current = (*Properties)[mPosition];  return true;}
            return false;
        }
        /// <summary>
        /// Returns a pointer to the next property or NULL if end of enumeration.
        /// </summary>
        /// <returns>Returns the next property or NULL if end of enumeration.</returns>
        inline Property* Next() { return MoveNext() ? Current : NULL; }
    };
}
