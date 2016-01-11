#pragma once

namespace Plato {

    class Point;
    class PointSpace;

    namespace PointRegexStatus {
        /// <summary>
        /// This enum is used to denote whether a regex point is special,
        /// such as being Wild or Unbounded (used for appending?).
        /// </summary>
        enum Enum {
            /// <summary>
            /// Indicates the regex point is a simple set of coordinates.
            /// </summary>
            Simple,
            /// <summary>
            /// Indicates the regex point is a set of points.
            /// </summary>
            Multivalued,
            /// <summary>
            /// Indicates the point is wild.
            /// </summary>
            Wild,
            /// <summary>
            /// Indicates the point is unbounded.
            /// </summary>
            Unbounded
        };
    }
    typedef PointRegexStatus::Enum PointRegexStatusEnum;

    /// <summary>
    /// A regex Point allows a range of points to be specified in terms of PointSpaces.
    /// </summary>
    class PointRegex {
    protected:
        /// <summary>
        /// The point space that corresponds to the regex point pattern.
        /// </summary>
        PointSpace& mPointSpace;

    public:
        /// <summary>
        /// Indicates the kind of regex point.
        /// </summary>
        PointRegexStatusEnum Status;

        /// <summary>
        /// Constructs a new regex point based on the specified point space.
        /// </summary>
        /// <param name="pointSpace">The point space that corresponds to the regex point pattern.</param>
        PointRegex(PointSpace& pointSpace);

        /// <summary>
        /// Produce a string representation of regex point.
        /// </summary>
        /// <returns>Returns the PointRegexStatusEnum as an integer enclosed in square brackets.</returns>
        string& StatusReport() const;

        /// <summary>
        /// Determines whether the given point would match this regex point.
        /// </summary>
        /// <remarks>
        /// A NULL point will only match a NULL regex point space.
        /// </remarks>
        /// <param name="point">The point to check for a match.</param>
        /// <returns>Returns true if the point matches.</returns>
        inline bool Match(const Point& point) const { return mPointSpace.IsContained(point); }

        /// <summary>
        /// Determines whether the given regex point would match this regex point.
        /// </summary>
        /// <remarks>
        /// A NULL point will only match a NULL regex point space.
        /// </remarks>
        /// <param name="regexPoint">The regex point to check for a match.</param>
        /// <returns>Returns true if the regex points match.</returns>
        bool PatternMatch(const PointRegex& regexPoint) const;
    };
}
