#pragma once

namespace Plato {

    class Point;
    class Locator;

    /// <summary>
    /// Specifies a location either in terms of a locator or via a point.
    /// If a locator is used, it is lazily converted to a fixed point
    /// with respect to the geometry of the parental container.
    /// </summary>
    class Location {
    public:
        /// <summary>
        /// The location as a point within the parent container.
        /// </summary>
        Point* PointPosition;
        /// <summary>
        /// The location in terms of a locator relative to the parent container.
        /// </summary>
        Locator* LocatorPosition;
        /// <summary>
        /// Flag indicating whether the location is a regex.
        /// </summary>
        bool IsRegex;

    public:
        /// <summary>
        /// Construct a new Location based on a locator.
        /// </summary>
        /// <param name="locatorPosition">The relative location fixing locator.</param>
        Location(Locator& locatorPosition);

        /// <summary>
        /// Construct a new Location based on a fixed point.
        /// </summary>
        /// <param name="pointPosition">The location as a point relative to parent container.</param>
        Location(Point& pointPosition);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        ~Location();

        /// <summary>
        /// Compares the location to another using a Euclidean distance metric.
        /// </summary>
        /// <remarks>Returns some distance measure. DEBUG: needs to be a real number.</remarks>
        /// <param name="location">The object to compare with.</param>
        /// <returns>Returns &lt; 0, if this &lt; location; 0 if this == location;
        /// &gt; 0, if this &gt; location.</returns>
        int CompareTo(Location& location);
    };
}
