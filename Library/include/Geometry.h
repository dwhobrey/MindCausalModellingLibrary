#pragma once

namespace Plato {

    class Point;
    class PointSpace;
    class Bounds;
    class Scale;
    class Distribution;
    class Container;
    class PointEnumerator;

    /// <summary>
    /// Geometry base class for modelling the boundary/shape/structure of an element.
    /// </summary>
    /// <remarks>
    /// The structure of an element is modelled in terms of an underlying coordinate system geometry
    /// and point distribution. There are three kinds of point geometries:
    /// <list type="bullet">
    /// <item><description>Cartesian Geometry</description></item>
    /// <item><description>Cylindrical Geometry</description></item>
    /// <item><description>Spherical Geometry</description></item>
    /// </list>
    /// Various geometric shapes are then derived from an appropriate point geometry, e.g.
    /// Pipe, Box, Layer, Disc, Sphere.
    /// <para>
    /// An element's geometric shape is then specified by setting its geometry property to one
    /// of the geometric shapes.
    /// </para>
    /// <para>
    /// All supported geometries have three spatial coordinates.
    /// A fourth overlay coordinate is added to distinguish elements that have the same spatial position.
    /// See the description of the Point for the orientation of axes.
    /// </para>
    /// </remarks>
    class Geometry {
#pragma region // Fields.
    public:
        /// <summary>
        /// A point representing a point outside the Cartesian space, i.e. an out of range point.
        /// </summary>
        static const Point* OutOfRangePoint;
        /// <summary>
        /// A point representing an unbounded point within the space.
        /// </summary>
        static const Point* UnboundedPoint;

        /// <summary>
        /// A point representing the upper limit of the Cartesian space.
        /// </summary>
        static const Point* UpperLimitPoint;

        /// <summary>
        /// A point representing the lower limit of the Cartesian space.
        /// </summary>
        static const Point* LowerLimitPoint;

        /// <summary>
        /// A point with all coordinates set to zero.
        /// </summary>
        static const Point* ZeroPoint;
#pragma endregion
    public:
        /// <summary>
        /// The local range of the dimensions of this shape in absolute units.
        /// </summary>
        /// <remarks>
        /// These are the internal dimensions, i.e. range, of the element.
        /// Unless it is scaled, the element occupies a unit cube in the parent element.
        /// </remarks>
        Bounds* InternalBounds;

        /// <summary>
        /// Fractional scaling factors for determining the shape's spatial extension in the parent space.
        /// </summary>
        /// <remarks>
        /// The spatial extension = mScale * Parent.mSize, and [0 &lt;= mScale &lt;= 1.0].
        /// <para>
        /// By convention a shape occupies a unit cube in the parent space when the scaling
        /// factors all have a value of 0, this is equivalent to (mScale == 1/Parent.mDimensionBounds.mSize).
        /// </para>
        /// </remarks>
        Scale* SpatialScale;

        /// <summary>
        /// Represents a rotational vector through which to rotate the axes relative to the parent z-axis.
        /// </summary>
        Point* Orientation;

        /// <summary>
        /// Point density distribution.
        /// </summary>
        Distribution* PointDistribution;

        /// <summary>
        /// The container associated with this geometry.
        /// </summary>
        Container* GeometryContainer;

        /// <summary>
        /// Constructor for specifying shape size, but not location,
        /// which would be fixed later depending on instance.
        /// </summary>
        /// <param name="bounds">The internal dimensions of the shape.</param>
        /// <param name="scale">The fractional extension of this shape in the parent element.</param>
        /// <param name="orientation">The orientation of the geometry z-axis relative to the parent z-axis.</param>
        /// <param name="distribution">Specifies how points are distributed in the geometry.</param>
        Geometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="geometry">The geometry to deep copy.</param>
        Geometry(const Geometry& geometry);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Geometry();

        /// <summary>
        /// Getter for the upper value of the coordinates.
        /// </summary>
        /// <returns>Returns a point whose coordinate values are the maximum permitted value for a coordinate.</returns>
        virtual const Point& GetUpperLimitPoint();

        /// <summary>
        /// Getter for the lower value of the coordinates.
        /// </summary>
        /// <returns>Returns a point whose coordinate values are the minimum permitted value for a coordinate.</returns>
        virtual const Point& GetLowerLimitPoint();

        /// <summary>
        /// Tests whether any coordinate is out of range in point.
        /// </summary>
        /// <remarks>
        /// Operations causing a coordinate to go out of range must
        /// set the Point.Status property, which is then a more efficient
        /// way of checking a point's status.
        /// </remarks>
        /// <param name="point">The point to check.</param>
        /// <returns>Returns true if a coordinate is out of range.</returns>
        bool IsOutOfRange(const Point& point);

        /// <summary>
        /// Retrieves a point according to the specified operation on a shape's
        /// point distribution.
        /// </summary>
        /// <remarks>
        /// When pointSpace is non-NULL, the New operation effectively returns
        /// vacant positions, if any, otherwise NULL.</remarks>
        /// <param name="operation">The operation to perform.</param>
        /// <param name="pointSpace">The space over which to apply the operation,
        /// set to NULL for whole space.</param>
        /// <returns>Returns a point according to a shapes coordinate system.</returns>
        virtual Point* GetPoint(CoordinateOperationsEnum operation, const PointSpace* pointSpace);

        /// <summary>
        /// Retrieves a coordinate value according to the specified operation on a shape's
        /// point distribution.
        /// </summary>
        /// <param name="operation">The operation to perform.</param>
        /// <param name="coordinate">The coordinate whose value is required.</param>
        /// <param name="pointSpace">The space over which to apply the operation,
        /// set to NULL for whole space.</param>
        /// <returns>Returns the required coordinate value as a float.</returns>
        virtual float GetCoordinate(CoordinateOperationsEnum operation, CoordinateNamesEnum coordinate, const PointSpace* pointSpace);

        /// <summary>
        /// Determines the number of points this geometry supports in the given point space.
        /// </summary>
        /// <param name="pointSpace">The point space whose point capacity is required.</param>
        /// <returns>Returns the number of points allowed, or 0 if none.</returns>
        virtual int GetCapacity(const PointSpace& pointSpace);

        /// <summary>
        /// Determines the number of container points currently in the point space.
        /// </summary>
        /// <remarks>
        /// Generalise to Indentifier regex and put in Container class.
        /// </remarks>
        /// <param name="pointSpace">The point space in which to count the points.</param>
        /// <returns>Returns the number of current points in the space, if any.</returns>
        virtual int Count(const PointSpace& pointSpace);

        /// <summary>
        /// Provides an enumeration over the coordinates of the geometric space.
        /// </summary>
        /// <remarks>This default implementation returns NULL. Derived classes must implement.</remarks>
        /// <returns>Returns an enumeration of Points.</returns>
        virtual PointEnumerator* GetEnumerator();

        /// <summary>
        /// Copies object.
        /// </summary>
        /// <remarks>
        /// Derived classes should override if they have additional instance members.
        /// </remarks>
        /// <returns>Returns a deep copy.</returns>
        virtual Geometry* Clone() const;

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
