#pragma once

namespace Plato {

    class UnivariateDistribution;
    class UnivariateDistributionEnumerator;

    /// <summary>
    /// In a uniform distribution the points are equally spaced across [0.0,1.0].
    /// </summary>
    class UniformUnivariateDistribution : public UnivariateDistribution {
    protected:
        /// <summary>
        /// The value used to normalise the distribution values based its capacity, essentially it's 1/Capacity.
        /// </summary>
        float mDivisor;
    
    public:
        /// <summary>
        /// Construct a new UniformDistribution instance.
        /// </summary>
        /// <param name="capacity">Capacity of new distribution.</param>
        UniformUnivariateDistribution(int capacity=UnivariateDistribution::UnboundedCapacity);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="distribution">The distribution to copy.</param>
        UniformUnivariateDistribution(const UniformUnivariateDistribution& distribution);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~UniformUnivariateDistribution();

        /// <summary>
        /// Set the capacity of the distirbution, i.e. the number of points it can contain.
        /// </summary>
        /// <param name="value">The new capacity value.</param>
        virtual void SetCapacity(int value);

        /// <summary>
        /// Get a new enumerator for this distribition instance.
        /// </summary>
        /// <returns>Returns a distribution enumerator.</returns>
        virtual UnivariateDistributionEnumerator* GetEnumerator();

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
    };

    /// <summary>
    /// Type safe convenience class for constructing uniform distribution enumerators.
    /// </summary>
    class UniformUnivariateDistributionEnumerator : public UnivariateDistributionEnumerator {
    public:
        /// <summary>
        ///  Construct a new enumerator for the uniform distribution.
        /// </summary>
        /// <param name="distribution">The uniform distribution for which an enumerator is required.</param>
        UniformUnivariateDistributionEnumerator(UniformUnivariateDistribution& distribution);
    };
}
