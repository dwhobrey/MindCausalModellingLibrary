#pragma once

namespace Plato {

    class Point;
    class Container;
    class Locator;

    /// <summary>
    /// DEBUG: Need locators for each coordinate type on each geometry shape.
    /// Tedious to declare explicitly.
    /// </summary>
    class PointLocator : public Locator {
    public:
        /// <summary>
        /// The operation to perform.
        /// </summary>
        CoordinateOperationsEnum Operation;

        /// <summary>
        /// Constructs a new locator that performs the specified operation.
        /// </summary>
        /// <param name="operation">The point operation to perform on desitnation geometry.</param>
        PointLocator(CoordinateOperationsEnum operation);

        /// <summary>
        /// Computes the point location for this locator.
        /// </summary>
        /// <param name="parentElement">The parent container with respect to which the
        /// locator refers.</param>
        /// <returns>Returns the location as a Point object.</returns>
        virtual Point* ToPoint(Container& parentElement);
    };
}
