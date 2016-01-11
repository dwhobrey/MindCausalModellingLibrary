#pragma once

namespace Plato {

    class Point;
    class Bounds;

    /// <summary>
    /// Class for objects that contain Points.
    /// Allows a user to determine if the space contains points.
    /// </summary>
    class PointSpace {
    public:   
        /// <summary>
        /// Default constructor.
        /// </summary>
        PointSpace();

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~PointSpace();
        
        /// <summary>
        /// Determines whether this point space would contain the specified point.
        /// </summary>
        /// <param name="point">The point to check for membership.</param>
        /// <returns>Returns true if the point falls within the boundaries of the space.</returns>
        virtual bool IsContained(const Point& point);

        /// <summary>
        /// The volume of the space as a fraction of parent space.
        /// </summary>
        /// <returns>Returns the fractional volume as a number in [0.0,1.0].</returns>
        virtual float Volume();

        /// <summary>
        /// The smallest box that contains the point space.
        /// </summary>
        /// <remarks>
        /// Note that the point space may be spherical or a disjoint set of spaces.
        /// For points, the dimensions (e.g. width) of the box will be zero.
        /// </remarks>
        /// <returns>Returns the bounds of the box.</returns>
        virtual Bounds* BoundingBox();
    };
}
