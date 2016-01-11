#pragma once

namespace Plato {
    /// <summary>
    /// Base class for an enumerator over Points.
    /// </summary>
    class PointEnumerator {
    public:
        /// <summary>
        /// Returns a pointer to the current point, or NULL if not set yet.
        /// </summary>
        Point* Current;
        /// <summary>
        /// Default constructor, sets Current to NULL.
        /// </summary>
        PointEnumerator() { Current = NULL; }
        /// <summary>
        /// Virtual destructor so that derived classes can clean up.
        /// </summary>
        virtual ~PointEnumerator() { }
        /// <summary>
        /// Advances the enumerator to the next item in the collection.
        /// </summary>
        virtual bool MoveNext() { return false; }
        /// <summary>
        /// Sets the enumerator to its initial position before the first item in the collection.
        /// </summary>
        virtual void Reset() { Current = NULL; }
        /// <summary>
        /// Returns a pointer to the next point or NULL if end of enumeration.
        /// </summary>
        /// <remarks>
        /// Calls MoveNext() and then Current.
        /// Do not use this method if the collection contains nulls.
        /// </remarks>
        /// <returns>Returns the next point or NULL if end of enumeration.</returns>
        inline Point* Next() { return MoveNext() ? Current : NULL; }
    };
}
