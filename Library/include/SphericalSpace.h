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
    /// Models a Spherical space.
    /// </summary>
    class SphericalSpace : public Geometry {
    public:
        /// <summary>
        /// A point representing an unbounded point within the Spherical space.
        /// </summary>
        static const Point* UnboundedPoint;
        /// <summary>
        /// A point representing the upper limit of the Spherical space.
        /// </summary>
        static const Point* UpperLimitPoint;

        /// <summary>
        /// A point representing the lower limit of the Spherical space.
        /// </summary>
        static const Point* LowerLimitPoint;

        /// <summary>
        /// A point with all coordinates set to zero.
        /// </summary>
        static const Point* ZeroPoint;

        /// <summary>
        /// Accessor for the "upper limit" point.
        /// </summary>
        virtual const Point& GetUpperLimitPoint();

        /// <summary>
        /// Accessor for the "lower limit" point.
        /// </summary>
        virtual const Point& GetLowerLimitPoint();

        /// <summary>
        /// Position in spherical coordinates, p = (radius,phi,theta).
        /// x = radius sin theta cos phi.
        /// y = radius sin theta sin phi.
        /// z = radius cos theta.
        /// r = (x**2 + y**2 + z**2)**(1/2).
        /// phi = arc tan (y / x). Note, +/- n * PI/2 depending on sign of x and y, since arc tan in [-pi/2,pi/2].
        /// theta = arc cos ( z / radius ).
        /// </summary>
        /// <param name="radius">The length of the position vector, [0,+Inf].</param>
        /// <param name="phi">Anti-clockwise polar angle of vector projected on xy-plane from positive x-axis, [0,2pi].</param>
        /// <param name="theta">Clockwise angle between the vector and positive z-axis, [0,pi].</param>
        /// <param name="overlayID">Overlay ID for new point, or zero if not required.</param>
        Point& SphericalPoint(float radius, float phi, float theta, float overlayID);

        /// <summary>
        /// Convert Spherical point to a Cartesian system.
        /// </summary>
        /// <param name="sphericalPoint">The spherical point to map to a Cartesian point.</param>
        /// <returns>Returns a cached point with respect to a Cartesian frame of reference.</returns>
        virtual Point& ToCartesian(Point& sphericalPoint);

    protected:
        /// <summary>
        /// Constructor for Spherical spaces, simply calls the base class.
        /// </summary>
        /// <param name="bounds"><see cref="Geometry"/></param>
        /// <param name="scale"><see cref="Geometry"/></param>
        /// <param name="orientation"><see cref="Geometry"/></param>
        /// <param name="distribution"><see cref="Geometry"/></param>
        SphericalSpace(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution);

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
