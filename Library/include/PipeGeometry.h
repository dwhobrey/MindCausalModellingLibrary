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
    /// Models a (one-dimensional) pipe shape.
    /// </summary>
    /// <remarks>
    /// The default orientation is along the X axis. Use the orientation property for other directions.
    /// </remarks>
    class PipeGeometry : public Geometry {
    private:
        /// <summary>
        /// The point distribution for this shape instance.
        /// </summary>
        Distribution* mDX;

        /// <summary>
        /// Used to allocate new points, <see cref="Plato::PipeGeometry::GetPoint"/>.
        /// </summary>
        DistributionEnumerator* mEX;

    public:
        /// <summary>
        /// General constructor for creating a new pipe shape instance.
        /// </summary>
        /// <param name="bounds">The length of the pipe. The Y and Z coordinates should be set to 1.</param>
        /// <param name="scale"><see cref="Geometry"/></param>
        /// <param name="orientation"><see cref="Geometry"/></param>
        /// <param name="distribution"><see cref="Geometry"/></param>
        /// <remarks>
        /// It is more convenient, and portable, to use one of the specialised constructors,
        /// such as <c>PipeGeometry(int capacityX)</c>.
        /// </remarks>
        PipeGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution);

        /// <summary>
        /// Constructs a new pipe where the size equates to its capacity.
        /// </summary>
        /// <remarks>The size is an integer value.</remarks>
        /// <param name="capacityX">The maximum number of points this pipe can contain.</param>
        PipeGeometry(int capacityX);

        /// <summary>
        /// Constructs a new pipe where the size equates to its capacity (rounded down to interger).
        /// </summary>
        /// <remarks>The size may be a non-integer.</remarks>
        /// <param name="capacityX">The maximum number of points this pipe can contain.</param>
        PipeGeometry(float capacityX);

        /// <summary>
        /// Release resources.
        /// </summary>
        ~PipeGeometry();

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

