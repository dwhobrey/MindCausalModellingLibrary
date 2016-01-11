#pragma once

namespace Plato {

    class PathNode;

    namespace PointStatus {
        /// <summary>
        /// This enum is used to denote whether a point is special,
        /// such as being out of range or wild.
        /// </summary>
        enum Enum {
            /// <summary>
            /// Indicates the point is a simple set of coordinates.
            /// </summary>
            Normal,
            /// <summary>
            /// Indicates the point is out of range.
            /// </summary>
            OutOfRange,
            /// <summary>
            /// Indicates the point is unbounded.
            /// </summary>
            Unbounded
        };
    }
    typedef PointStatus::Enum PointStatusEnum;

    /// <summary>
    /// Point class for modelling a spatial point in a coordinate system determined by geometry.
    /// This implementation models a Cartesian coordinate point.
    /// </summary>
    class Point {

#pragma region // Fields.
    public:
        /// <summary>
        /// Number of coordinates.
        /// All coordinate systems have 4 dimensions indexed {0,1,2,3}.
        /// Indices {0,1,2} specify the spatial position.
        /// Index 3 specifies the overlay ID for the point,
        /// i.e. there may be multiple points at the same position, which are distinguished via this coordinate.
        /// </summary>
        static const int NumberOfCoordinates = 4;

        /// <summary>
        /// Number of spatial coordinates, i.e. the number of coordinates minus one.
        /// </summary>
        static const int NumberOfSpatialCoordinates = 3;

        /// <summary>
        /// Offset of overlay field in coordinate array.
        /// </summary>
        static const int OverlayField = 3;

        /// <summary>
        /// The separator used before each coordinate in a
        /// position value when in the string form of a point.
        /// </summary>
        static const char PositionSeparator = '_';

        /// <summary>
        /// This pattern is used to denote an out of range point.
        /// </summary>
        static const string* OutOfRangePatternString;

        /// <summary>
        /// This pattern is used to denote an unbounded point.
        /// </summary>
        static const string* UnboundedPatternString;
#pragma endregion

#pragma region // Instance Variables.
    public:
        /// <summary>
        /// Holds the coordinate tuple.
        /// </summary>
        float Coordinates[NumberOfCoordinates];

        /// <summary>
        /// The kind of coordinate values the point contains.
        /// </summary>
        PointStatusEnum Status;
        
        /// <summary>
        /// Hashcode for coordinate values.
        /// </summary>
        size_t HashCode;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// Construct a new point object.
        /// Position in Cartesian coordinates, p = (x,y,z).
        /// Axes orientation as follows:
        /// x-axis = horizontal axis with positive direction intercepting the observer, decreasing away from observer.
        /// y-axis = horizontal axis with positive direction perpendicular to observer, increasing left to right.
        /// z-axis = vertical axis with positive direction in an upward direction.
        /// </summary>
        /// <param name="x">Position on x-axis, [-Inf,+Inf].</param>
        /// <param name="y">Position on y-axis, [-Inf,+Inf].</param>
        /// <param name="z">Position on z-axis, [-Inf,+Inf].</param>
        /// <param name="overlayID">Overlay ID for new point, or zero if not required.</param>
        Point(float x=0.0f, float y=0.0f, float z=0.0f, float overlayID=0.0f);

        /// <summary>
        /// Construct a new point object with an particular status.
        /// </summary>
        /// <remarks>This is used to construct the standard Unbounded and OutOfRange points.</remarks>
        /// <param name="status">The status of the point.</param>
        /// <param name="x">Position on x-axis.</param>
        /// <param name="y">Position on y-axis.</param>
        /// <param name="z">Position on z-axis.</param>
        /// <param name="overlayID">Overlay ID for new point.</param>
        Point(PointStatusEnum status, float x=0.0f, float y=0.0f, float z=0.0f, float overlayID=0.0f);

        /// <summary>
        /// Constructs a Point by parsing a string position.
        /// <list type="number">
        /// <item><description>position : { '_' float } * | '_' pattern ]</description></item>
        /// <item><description>pattern : OutOfRangePattern | UnboundedPattern</description></item>
        /// </list>
        /// </summary>
        /// <param name="position">The string form of a position to parse.</param>
        Point(const string& position);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="point">The point to copy.</param>
        Point(const Point& point);
#pragma endregion

    public:
        /// <summary>
        /// Refreshes the hash code for the coordinate values.
        /// </summary>
        /// <remarks>
        /// The hash code is updated in point constructors,
        /// but must be called by user code if coordinate values
        /// are subsequently changed.
        /// </remarks>
        void UpdateHashCode();

        /// <summary>
        /// Produce a string representation of point.
        /// </summary>
        /// <remarks>This is used to produce the Position of an Identifier.
        /// By convention zero valued trailing coordinates are not included.
        /// So if all coordinates are zero, the empty string is returned.
        /// Note, text patterns are used to denote Unbounded and OutOfRange.</remarks>
        /// <returns>Returns the coordinates of the point, each coordinate is preceeded by a '_':
        /// "_c0_c1_c2_overlayID". Trailing zero valued coordinates are trimmed.
        /// </returns>
        string& StatusReport() const;

        /// <summary>
        /// Tests whether coordinates are equal.
        /// </summary>
        /// <param name="point">Point to compare with.</param>
        /// <returns>Returns true if equal.</returns>
        bool Equals(const Point& point) const;

        /// <summary>
        /// Tests whether the spatial coordinates are equal.
        /// Note: could add a more sophisticated version that transform to Cartesian first if types differ,
        /// such as PositionEquals(Point tc).
        /// </summary>
        /// <param name="tc">Point to compare with.</param>
        /// <returns>Returns true if equal.</returns>
        bool SpatialCoordinatesEquals(const Point& tc) const;

        /// <summary>
        /// Tests whether the overlay IDs are the same.
        /// </summary>
        /// <param name="tc">Point to compare with.</param>
        /// <returns>Returns true if points have the same overlay ID.</returns>
        bool OverlayEquals(const Point& tc) const;

        /// <summary>
        /// Tests whether all coordinates are equal.
        /// Checks both spatial and overlay coordinate values.
        /// </summary>
        /// <param name="tc">Point to compare with.</param>
        /// <returns>Returns true if equal.</returns>
        bool CoordinatesEquals(const Point& tc) const;

        /// <summary>
        /// Compares the point to another using a Euclidean distance metric.
        /// </summary>
        /// <param name="point">The point to compare with.</param>
        /// <remarks>Returns some distance measure. DEBUG: needs to be a real number.</remarks>
        /// <returns>Returns &lt; 0, if this &lt; pint; 0 if this == pint; &gt; 0, if this &gt; point. </returns>
        int CompareTo(const Point& point) const;

        /// <summary>
        /// Compares two points, allowing either to be NULL.
        /// </summary>
        /// <param name="a">The first point to compare.</param>
        /// <param name="b">The second point to compare.</param>
        /// <returns>Returns &lt; 0, if a &lt; b; 0 if a == b; &gt; 0, if a &gt; b.</returns>
        static int Compare(const Point* a, const Point* b);

        /// <summary>
        /// Returns a PathNode with the given name and a filter for the specified coordinate.
        /// Usage: Path tmp = Path.Absolute / "test" + Point.Node("fred",12) + Path.Node("john",14,10,12);
        /// </summary>
        /// <param name="name">The name to give the path node.</param>
        /// <param name="coordinates">The coordinates to filter on.</param>
        /// <returns>Returns a PathNode with the given name and coordiantes filter.</returns>
        static PathNode* Node(string& name, float coordinates);

    private:
        friend class InitializerCatalogue;
        /// <summary>
        /// The class initializer.
        /// </summary>
        static void Initializer();
        /// <summary>
        /// The class finalizer.
        /// </summary>
        static void Finalizer();
    };
}
