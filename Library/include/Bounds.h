#pragma once

namespace Plato {

    class Size;
    class Point;

    /// <summary>
    /// Bounds class for holding the bounds of an element.
    /// Specifies the min and max coordinate values for an element.
    /// Upperbound is calculated via LowerBound + Width;
    /// </summary>
    class Bounds {
    public:
        /// <summary>
        /// The size of the bounded area.
        /// </summary>
        Size* Width;
        /// <summary>
        /// The lower bound for the area.
        /// </summary>
        Point* LowerBound;

        /// <summary>
        /// Construct a new Bounds object of the specified width and lower bound.
        /// </summary>
        /// <param name="lowerBound">The lower bound given as a Point.</param>
        /// <param name="width">The width of the bounded area.</param>
        Bounds(Point& lowerBound, Size& width);

        /// <summary>
        /// Construct a new Bounds object of the specified width.
        /// </summary>
        /// <remarks>The lower bound is set to Point.ZeroPoint.</remarks>
        /// <param name="width">The width of the bounded area.</param>
        Bounds(Size& width);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        Bounds(const Bounds& bounds);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        ~Bounds();
    };
}
