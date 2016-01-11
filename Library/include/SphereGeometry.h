#pragma once

namespace Plato {

    class Geometry;
    class Distribution;
    class DistributionEnumerator;
    class Bounds;
    class Scale;
    class Point;
    class PointSpace;
    class SphericalSpace;
    
    /// <summary>
    /// Models a three dimensional elliptical shape (e.g. a sphere spanning a discrete volume).
    /// </summary>
    /// <remarks>The default orientation is along the X, Y, Z axes. Use the orientation property for other orientations, etc.
    /// </remarks>
    class SphereGeometry : public SphericalSpace {
    public:
        /// <summary>
        /// General constructor for creating a new sphere shape instance.
        /// </summary>
        /// <param name="bounds">The X, Y and Z diameters of the elliptoid.</param>
        /// <param name="scale"><see cref="Geometry"/></param>
        /// <param name="orientation"><see cref="Geometry"/></param>
        /// <param name="distribution"><see cref="Geometry"/></param>
        /// <remarks>
        /// It is more convenient, and portable, to use one of the specialised constructors,
        /// such as <c>SphereGeometry(int capacityX, int capacityY, int capacityZ)</c>.
        /// </remarks>
        SphereGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution);
    };
}
