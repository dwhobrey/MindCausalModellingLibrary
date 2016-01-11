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
    /// Models a two dimensional layer shape (i.e. discrete planar field).
    /// </summary>
    /// <remarks>The default orientation is in the XY plane. Use the orientation property for Z oriented layers, etc.
    /// </remarks>
    class LayerGeometry : public Geometry {
    private:
        /// <summary>
        /// The point X axis distribution for this shape instance.
        /// </summary>
        Distribution* mDX;

        /// <summary>
        /// Used to allocate new points in conjunction with mEY, <see cref="Plato::PipeGeometry::GetPoint"/>.
        /// </summary>
        DistributionEnumerator* mEX;

        /// <summary>
        /// The point Y axis distribution for this shape instance.
        /// </summary>
        Distribution* mDY;

        /// <summary>
        /// Used to allocate new points in conjunction with mEX, <see cref="Plato::PipeGeometry::GetPoint"/>.
        /// </summary>
        DistributionEnumerator* mEY;

    public:
        /// <summary>
        /// General constructor for creating a new layer shape instance.
        /// </summary>
        /// <param name="bounds">The X and Y length of the layer. The Z coordinate should be set to 1.</param>
        /// <param name="scale"><see cref="Geometry"/></param>
        /// <param name="orientation"><see cref="Geometry"/></param>
        /// <param name="distribution"><see cref="Geometry"/></param>
        /// <remarks>
        /// It is more convenient, and portable, to use one of the specialised constructors,
        /// such as <c>LayerGeometry(int capacityX, int capacityY)</c>.
        /// </remarks>
        LayerGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution);

        /// <summary>
        /// Constructs a new layer where the size (area) equates to its capacity.
        /// </summary>
        /// <remarks>The size dimensions are integer values.</remarks>
        /// <param name="capacityX">The maximum number of points in the X dimension this layer can contain.</param>
        /// <param name="capacityY">The maximum number of points in the Y dimension this layer can contain.</param>
        LayerGeometry(int capacityX, int capacityY);

        /// <summary>
        /// Initialises and resets the pipe and new point allocator.
        /// </summary>
        void Reset();

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
        /// <returns>Returns a point derived from Point according to a shapes
        /// coordinate system.</returns>
        virtual Point* GetPoint(CoordinateOperationsEnum operation, const PointSpace& pointSpace);
    };
}
