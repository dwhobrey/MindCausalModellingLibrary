#pragma once

namespace Plato {

    class Geometry;
    class Distribution;
    class DistributionEnumerator;
    class Bounds;
    class Scale;
    class Point;
    class PointSpace;

    /// <summary>
    /// Models a three dimensional box shape (i.e. discrete volume).
    /// </summary>
    /// <remarks>The default orientation is along the X,Y,Z axes. Use the orientation property other orientations.
    /// </remarks>
    class BoxGeometry : public Geometry {
    public:
        /// <summary>
        /// General constructor for creating a new box shape instance.
        /// </summary>
        /// <param name="bounds">The X, Y and Z length of the box.</param>
        /// <param name="scale"><see cref="Geometry"/></param>
        /// <param name="orientation"><see cref="Geometry"/></param>
        /// <param name="distribution"><see cref="Geometry"/></param>
        /// <remarks>
        /// It is more convenient, and portable, to use one of the specialised constructors,
        /// such as <c>BoxGeometry(int capacityX, int capacityY, int capacityZ)</c>.
        /// </remarks>
        BoxGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution);
    };
}

