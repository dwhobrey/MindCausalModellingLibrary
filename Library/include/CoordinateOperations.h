#pragma once

namespace Plato {
    namespace CoordinateOperations {
        /// <summary>
        /// This provides an enumeration of the kinds of point
        /// and coordinate values that can be retrieved from the
        /// context of a geometric shape.
        /// </summary>
        /// <remarks>
        /// Geometric shapes allocate points according to a distribution
        /// for which they maintain context information.
        /// Various point and coordinate values, defined by this enumeration,
        /// can be retrieved for a shape via the Geometry.GetCoordinate
        /// or Geometry.GetPoint methods.
        /// </remarks>
        enum Enum {
            /// <summary>
            /// Fetch the current available coordinate value(s).
            /// </summary>
            Current,
            /// <summary>
            /// Fetch the next available lower (e.g. first) coordinate value.
            /// </summary>
            Head,
            /// <summary>
            /// Fetch the next available coordinate value(s) relative to
            /// the current point.
            /// This is defined as being 1 unit more than the current point,
            /// according to the distribution function for the shape.
            /// </summary>
            Next,
            /// <summary>
            /// Fetch a new coordinate value(s) according to the
            /// distribution function for the shape.
            /// Unlike the Next operation, the new coordinate may be
            /// more or less than the current point and differ by a value
            /// other than unity.
            /// </summary>
            New,
            /// <summary>
            /// Fetch a new OverLayID coordinate value for the current point.
            /// Returns a new coordinate value or point, or OutOfRangePoint if full.
            /// </summary>
            OverLay,
            /// <summary>
            /// Generate a random coordinate or point according to the shape's
            /// distribution function.
            /// </summary>
            Random,
            /// <summary>
            /// Fetch the next available upper (e.g. last) coordinate value.
            /// </summary>
            Tail
        };
    }
    typedef CoordinateOperations::Enum CoordinateOperationsEnum;
}
