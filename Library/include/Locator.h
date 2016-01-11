#pragma once

namespace Plato {

    class Point;
    class Container;

    /// <summary>
    /// The base class for locators.
    /// </summary>
    /// <remarks>
    /// The locator classes are used for computing the position of elements in a deferred manner.
    /// For example, they are used as parameters to Distributors when an element's position
    /// is determined after distribution.
    /// </remarks>
    class Locator {
    public:
        /// <summary>
        /// Computes the point location for this locator.
        /// </summary>
        /// <param name="parentElement">The parent container with respect to which the
        /// locator refers.</param>
        /// <returns>Returns the location as a Point object.</returns>
        virtual Point* ToPoint(Container& parentElement);
    };
}
