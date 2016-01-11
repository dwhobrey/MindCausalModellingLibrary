#pragma once

namespace Plato {

    class DistributionEnumerator;
    class PointSpace;
    class Point;
    class UnivariateDistribution;

    /// <summary>
    /// Base class for representing normalised point distributions.
    /// </summary>
    /// <remarks>
    /// Distributions are used to determine the point distribution in a region or for an element
    /// by combining univariate scalar distributions.
    /// They are used in Distributors and Enumerators.
    /// The domain for all the standard distribution's point values is the closed interval [0.0,1.0].
    /// <para>
    /// Use-cases:
    /// <list type="number">
    /// <item><description>
    /// User wants to specify a sphere or cube with a normal distribution of points.
    /// </description></item>
    /// <item><description>
    /// Later, they want to enumerate the points in one hemisphere (filter).
    /// </description></item>
    /// <item><description>
    /// Later still, they want to randomly select (filter) N vacant points.
    /// </description></item>
    /// </list>
    /// </para>
    /// </remarks>
    class Distribution {
    protected:
        /// <summary>
        /// The maximum number of points in the distribution.
        /// </summary>
        int mCapacity;

#pragma region // Constructors.
    public:
        /// <summary>
        /// General constructor.
        /// Create a new distribution instance with the specified capacity.
        /// </summary>
        /// <param name="capacity"></param>
        Distribution(int capacity=UnivariateDistribution::UnboundedCapacity);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="distribution">The distribution to copy.</param>
        Distribution(const Distribution& distribution);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Distribution();
#pragma endregion

#pragma region // Accessors.
        /// <summary>
        /// Set the capacity of the distirbution, i.e. the number of points it can contain.
        /// </summary>
        /// <param name="value">The new capacity value.</param>
        virtual void SetCapacity(int value);
        /// <summary>
        /// Get the current capacity.
        /// </summary>
        /// <returns>Returns the current capacity.</returns>
        int Capacity();
#pragma endregion

        /// <summary>
        /// Creates an enumerator for the distribution.
        /// Sequence is repeatably deterministic and ordered by x coordinate, low to high.
        /// </summary>
        /// <returns>Returns an enumerator for the distribution.</returns>
        virtual DistributionEnumerator* GetEnumerator();

        /// <summary>
        /// Shallow copy this distribution instance.
        /// </summary>
        /// <remarks>Derived classes should override and call their copy constructor.</remarks>
        /// <returns>Returns a shallow copy.</returns>
        virtual Distribution* Clone() const;

        /// <summary>
        /// Retrieves a point according to the specified operation on a shape's
        /// point distribution.
        /// </summary>
        /// <param name="operation">The operation to perform.</param>
        /// <param name="pointSpace">The space over which to apply the operation,
        /// set to NULL for whole space.</param>
        /// <returns>Returns a point derived from Point according to a shapes
        /// coordinate system.</returns>
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
        virtual float GetCoordinate(CoordinateOperationsEnum operation, CoordinateNamesEnum coordinate, const PointSpace& pointSpace);
    };
}
