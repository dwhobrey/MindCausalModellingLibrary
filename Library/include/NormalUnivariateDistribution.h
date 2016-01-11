#pragma once

namespace Plato {

    class UnivariateDistribution;
    class UnivariateDistributionEnumerator;

    /// <summary>
    /// In a normal distribution the points are normally distributed across [0.0,1.0],
    /// according to the mean and sigma values.
    /// </summary>
    /// <remarks>
    /// For spherical coordinates a half distribution would be useful.
    /// Note: it doesn't make sense to have a UnboundedCapacity normal distribution.
    /// </remarks>
    class NormalUnivariateDistribution : public UnivariateDistribution {
    protected:
        /// <summary>
        /// The value used to normalise the distribution values based its capacity, essentially it's 1/Capacity.
        /// </summary>
        int mDivisor;
        /// <summary>
        /// A precomputation of the Gauss coefficient for a particular sigma.
        /// </summary>
        float mGaussCoeff;
    public:
        /// <summary>
        /// The standard deviation of the distribution.
        /// </summary>
        float Sigma;

        /// <summary>
        /// The mean (expected) value.
        /// </summary>
        float Mean;

        /// <summary>
        /// The highest range value.
        /// </summary>
        float YHigh;

        /// <summary>
        /// The lowest range value.
        /// </summary>
        float YLow;

        /// <summary>
        /// For a normalised pulse width: sigma = NormPulseCoeff / capacity.
        /// </summary>
        static const double NormPulseCoeff;

        /// <summary>
        /// The default Sigma value, i.e. 0.5.
        /// </summary>
        static const float DefaultSigma;

        /// <summary>
        /// The default mean value, i.e. 0.5.
        /// </summary>
        static const float DefaultMean;

        /// <summary>
        /// Creates a new NormalDistribition object with the required capacity, mean and sigma.
        /// </summary>
        /// <param name="capacity">The maximum number of points this distribution contains.</param>
        /// <param name="mean">The mean for the distribution, where 0.0 &lt; mean &lt; 1.0.</param>
        /// <param name="sigma">The standard deviation, where 0.0 &lt; sigma &lt; 1.0.</param>
        NormalUnivariateDistribution(int capacity=UnivariateDistribution::UnboundedCapacity,
            float mean=DefaultMean, float sigma=NormalisedSigma(UnivariateDistribution::UnboundedCapacity));
        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="distribution">The distribution to copy.</param>
        NormalUnivariateDistribution(const NormalUnivariateDistribution& distribution);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~NormalUnivariateDistribution();

        /// <summary>
        /// Computes a value for Sigma which makes the distribution width at half height
        /// proportional to 1/Capacity.
        /// </summary>
        /// <param name="capacity">The population size.</param>
        /// <returns>Returns normalised Sigma value.</returns>
        static float NormalisedSigma(int capacity);

        /// <summary>
        /// Computes the Gaussian of x for a given mean and sigma.
        /// </summary>
        /// <param name="x">The x value, where 0.0 &lt; = x &lt; = 1.0.</param>
        /// <returns>Returns the corresponding y value.</returns>
        double Gauss(float x);

        /// <summary>
        /// Computes the inverse of the Gaussian, where x = Mean +- InverseGauss(y).
        /// </summary>
        /// <param name="y">The y value, where 0.0 &lt; = y &lt; = 1.0.</param>
        /// <returns>Returns the deviation to add or subtract from the mean, giving the two possible x values.</returns>
        double InverseGauss(float y);

        /// <summary>
        /// Get a new enumerator for this distribition instance.
        /// </summary>
        /// <returns>Returns a distribution enumerator.</returns>
        virtual UnivariateDistributionEnumerator* GetEnumerator();

        /// <summary>
        /// Capacity setter that adjusts the Divisor when the capacity changes.
        /// </summary>
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
    };

    /// <summary>
    /// Type safe convenience class for constructing normal distribution enumerators.
    /// </summary>
    class NormalUnivariateDistributionEnumerator : public UnivariateDistributionEnumerator {
    public:
        /// <summary>
        ///  Construct a new enumerator for the normal distribution.
        /// </summary>
        /// <param name="distribution">The normal distribution for which an enumerator is required.</param>
        NormalUnivariateDistributionEnumerator(NormalUnivariateDistribution& distribution);
    };
}
