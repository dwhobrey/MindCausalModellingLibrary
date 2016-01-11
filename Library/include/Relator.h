#pragma once

namespace Plato {

    /// <summary>
    /// Relators are used by the converter class for specifying relational patterns
    /// that are used as constraints between the positions of elements when determining
    /// whether a converter is applicable.
    /// </summary>
    /// <remarks>
    /// <para>
    /// There are two categories of relators:
    /// proximity relators that model an N-arity relation between a set of elements,
    /// for example, a neighbourhood relation,
    /// and connector relators that model an M-N-arity relation between two sets of elements,
    /// for example, a set of input and a set of output elements that are to be interconnected.
    /// </para>
    /// <para>This base class currently only acts as a marker for relators.</para>
    /// </remarks>
    class Relator {
    protected:
        /// <summary>
        /// Only allow derived classes to construct relators.
        /// </summary>
        Relator();
        /// <summary>
        /// Deallocate relator.
        /// </summary>
        virtual ~Relator();
    };
}
