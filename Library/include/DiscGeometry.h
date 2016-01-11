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
    /// Models a two dimensional disc shape (i.e. a disc spanning a discrete planar field).
    /// </summary>
    /// <remarks>
    /// The default orientation is in the XY plane. Use the orientation property for Z oriented layers, etc.
    /// </remarks>
    class DiscGeometry : public SphericalSpace {
    public:
        /// <summary>
        /// General constructor for creating a new disc shape instance.
        /// </summary>
        /// <param name="bounds">The X and Y diameters of the disc. The Z coordinate should be set to 1.</param>
        /// <param name="scale"><see cref="Geometry"/></param>
        /// <param name="orientation"><see cref="Geometry"/></param>
        /// <param name="distribution"><see cref="Geometry"/></param>
        /// <remarks>
        /// It is more convenient, and portable, to use one of the specialised constructors,
        /// such as <c>DiscGeometry(int capacityX, int capacityY)</c>.
        /// </remarks>
        DiscGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution);
    };
}
