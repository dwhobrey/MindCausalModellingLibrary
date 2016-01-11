#pragma once

namespace Plato {

    class Distribution;
    class DistributionEnumerator;
    class PointSpace;
    class Point;
    class UnivariateDistribution;
    class NormalUnivariateDistribution;

    /// <summary>
    /// In a normal distribution the points are normally distributed across [0.0,1.0],
    /// according to the mean and sigma values.
    /// </summary>
    /// <remarks>
    /// For spherical coordinates a half distribution would be useful.
    /// Note: it doesn't make sense to have a UnboundedCapacity normal distribution.
    /// </remarks>
    class NormalDistribution : public Distribution {
    private:
        /// <summary>
        /// The cross-product of 3 univariate distributions is used to form distribution.
        /// </summary>
        NormalUnivariateDistribution* mUnivariateDistributions[3];

     public:
        /// <summary>
        /// Creates a new NormalDistribition object with the required capacity, mean and sigma.
        /// </summary>
        /// <param name="aCapacity">The maximum number of points this distribution contains in the A dimension.</param>
        /// <param name="bCapacity">The maximum number of points this distribution contains in the B dimension.</param>
        /// <param name="cCapacity">The maximum number of points this distribution contains in the C dimension.</param>
        /// <param name="mean">The mean for the distribution, where 0.0 &lt; mean &lt; 1.0.</param>
        /// <param name="sigma">The standard deviation, where 0.0 &lt; sigma &lt; 1.0.</param>
         NormalDistribution(int aCapacity=UnivariateDistribution::UnboundedCapacity, 
            int bCapacity=UnivariateDistribution::UnboundedCapacity, 
            int cCapacity=UnivariateDistribution::UnboundedCapacity, 
            float mean=NormalUnivariateDistribution::DefaultMean,
            float sigma=Numbers::UnboundedValue);
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="distribution">The distribution to copy.</param>
        NormalDistribution(const NormalDistribution& distribution);
        /// <summary>
        /// Get a new enumerator for this distribition instance.
        /// </summary>
        /// <returns>Returns a distribution enumerator.</returns>
        virtual DistributionEnumerator* GetEnumerator();
        /// <summary>
        /// Shallow copy this distribution instance.
        /// </summary>
        /// <remarks>Derived classes should override and call their copy constructor.</remarks>
        /// <returns>Returns a shallow copy.</returns>
        virtual Distribution* Clone() const;
    };

    /// <summary>
    /// Type safe convenience class for constructing normal distribution enumerators.
    /// </summary>
    class NormalDistributionEnumerator : public DistributionEnumerator {
    public:
        /// <summary>
        ///  Construct a new enumerator for the normal distribution.
        /// </summary>
        /// <param name="distribution">The normal distribution for which an enumerator is required.</param>
        NormalDistributionEnumerator(NormalDistribution& distribution);
    };
}
