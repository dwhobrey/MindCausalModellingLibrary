#pragma once

namespace Plato {

    /// <summary>
    /// This class represents a bit field for selecting coordinates.
    /// </summary>
    class CoordinateSelection {
    public:
        /// <summary>
        /// An array for holding the selection value for each dimension.
        /// </summary>
        bool Selections[3];

        /// <summary>
        /// Construct a new CoordinateSelection object.
        /// </summary>
        /// <param name="c0">True indicates first coordinate is selected.</param>
        /// <param name="c1">True indicates second coordinate is selected.</param>
        /// <param name="c2">True indicates third coordinate is selected.</param>
        CoordinateSelection(bool c0, bool c1, bool c2);
    };
}
