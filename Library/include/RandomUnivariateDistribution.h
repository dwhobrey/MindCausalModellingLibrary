#pragma once

namespace Plato {

    class UnivariateDistribution;
    class UnivariateDistributionEnumerator;
    class RandomUnivariateDistributionEnumerator;

    /// <summary>
    /// In a random distribution the points are randomly distributed across [0.0,1.0].
    /// </summary>
    /// <remarks>
    /// Note: currently an unbounded capacity random distribution is not supported
    /// so as to provide a simple lazy ordered enumerator without storing an unbounded distrn.
    /// </remarks>
    class RandomUnivariateDistribution : public UnivariateDistribution {
    protected:
        /// <summary>
        /// The sorted, but random, distribution values.
        /// </summary>
        vector<float>* mDistributionValues;

    public:
        /// <summary>
        /// Construct a new RandomDistribution instance.
        /// </summary>
        /// <param name="capacity">Capacity of new distribution. The default value
        /// will construct a new RandomDistribution instance with an unbounded capacity.
        /// </param>
        RandomUnivariateDistribution(int capacity=UnivariateDistribution::UnboundedCapacity);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="distribution">The distribution to copy.</param>
        RandomUnivariateDistribution(const RandomUnivariateDistribution& distribution);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~RandomUnivariateDistribution();

    public:
        /// <summary>
        /// Get a new enumerator for this distribition instance.
        /// </summary>
        /// <returns>Returns a distribution enumerator.</returns>
        virtual UnivariateDistributionEnumerator* GetEnumerator();

        /// <summary>
        /// Capacity setter that adjusts the Divisor when the capacity changes.
        /// </summary>
        /// <remarks>
        /// TO DO: if capacity is adjusted it recomputes the whole distribution, i.e. slow.
        /// It would be better to adjust the distribution up or down in size.
        /// In this case, must allow for clones of distribution that would need a deep copy of rnd too.
        /// </remarks>
        virtual void SetCapacity(int value);

        /// <summary>
        /// Shallow copy this distribution instance.
        /// </summary>
        /// <remarks>Derived classes should override and call their copy constructor.</remarks>
        /// <returns>Returns a shallow copy.</returns>
        virtual UnivariateDistribution* Clone() const;

        /// <summary>
        /// Indexer for getting the nth value of the distribution.
        /// </summary>
        virtual float operator[](int index);

        /// <summary>
        /// Resets the distribution.
        /// </summary>
        /// <remarks>
        /// Normally called internally by SetCapacity or in constructor.
        /// </remarks>
        void Reset();
    };

    /// <summary>
    /// Type safe convenience class for constructing random distribution enumerators.
    /// </summary>
    class RandomUnivariateDistributionEnumerator : public UnivariateDistributionEnumerator {
    public:
        /// <summary>
        ///  Construct a new enumerator for the random distribution.
        /// </summary>
        /// <param name="distribution">The random distribution for which an enumerator is required.</param>
        RandomUnivariateDistributionEnumerator(RandomUnivariateDistribution& distribution);

        virtual ~RandomUnivariateDistributionEnumerator();
    };
}
