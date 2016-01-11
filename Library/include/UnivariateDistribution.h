#pragma once

namespace Plato {

    class UnivariateDistributionEnumerator;

    /// <summary>
    /// Base class for representing normalised univariate scalar distributions.
    /// </summary>
    /// <remarks>
    /// Distributions are used to determine the point distribution in a region or for an element
    /// by combining univariate scalar distributions.
    /// They are used in Distributors and Enumerators.
    /// The domain for all the standard distributions is the closed interval [0.0,1.0].
    /// <para>
    /// Requirements:
    /// <list type="number">
    /// <item><description>
    /// The distribution should allow construction with an Unbounded capacity.
    /// </description></item>
    /// <item><description>
    /// It should allow the Capacity to be repeatedly adjusted later.
    /// </description></item>
    /// <item><description>
    /// It should allow clones to be made.
    /// </description></item>
    /// <item><description>
    /// It would be nice to support lazy unbounded distributions, i.e. no infinite arrays!
    /// </description></item>
    /// </list>
    /// </para>
    /// </remarks>
    class UnivariateDistribution {
#pragma region // Fields.
    public:
        /// <summary>
        /// The lowest x value of the distribution, fixed at 0.0.
        /// </summary>
        static const float MinimumX;

        /// <summary>
        /// The highest x value of the distribution, fixed at 1.0.
        /// </summary>
        static const float MaximumX;

        /// <summary>
        /// The lowest y value of the distribution, fixed at 0.0.
        /// </summary>
        static const float MinimumY;

        /// <summary>
        /// The highest y value of the distribution, fixed at 1.0.
        /// </summary>
        static const float MaximumY;

        /// <summary>
        /// Special distribution capacity value used to indicate that the number of points in distribution is unbounded.
        /// </summary>
        static const int UnboundedCapacity;

        /// <summary>
        /// An internal value for the largest allowable population size used in some cases when dealing with unbounded distributions.
        /// </summary>
        static const int MaximumCapacity;
#pragma endregion

#pragma region // Instance variables.
    protected:
        /// <summary>
        /// The maximum number of points in the distribution.
        /// </summary>
        int mCapacity;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// General constructor.
        /// Create a new distribution instance with the specified capacity.
        /// </summary>
        /// <param name="capacity">The capacity of the distribution.
        /// The default value creates a new distirbution with an unbounded capacity.
        /// </param>
        UnivariateDistribution(int capacity=UnboundedCapacity);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="distribution">The distribution to copy.</param>
        UnivariateDistribution(const UnivariateDistribution& distribution);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~UnivariateDistribution();
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
        virtual UnivariateDistributionEnumerator* GetEnumerator();

        /// <summary>
        /// Shallow copy this distribution instance.
        /// </summary>
        /// <remarks>Derived classes should override and call their copy constructor.</remarks>
        /// <returns>Returns a shallow copy.</returns>
        virtual UnivariateDistribution* Clone() const;

        /// <summary>
        /// An indexer for accessing the distribution values.
        /// Index in {0..(Capacity-1)}.
        /// </summary>
        virtual float operator[](int index);
    };
}
