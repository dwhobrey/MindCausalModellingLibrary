#pragma once

namespace Plato {

    class Distribution;
    class DistributionEnumerator;
    class PointSpace;
    class Point;
    class UnivariateDistribution;
    class UniformUnivariateDistribution;

    /// <summary>
    /// In a uniform distribution the points are equally spaced across [0.0,1.0].
    /// </summary>
    class UniformDistribution : public Distribution {
    protected:
        // DEBUG: Sort out capacity representation: base 3d too?
        /// <summary>
        /// The individual capacities of the dimensions.
        /// </summary>
        int mCapacities[3];
    
    public:
        /// <summary>
        /// Creates a new UniformDistribition object with the required capacity, mean and sigma.
        /// </summary>
        /// <param name="aCapacity">The maximum number of points this distribution contains in the A dimension.</param>
        /// <param name="bCapacity">The maximum number of points this distribution contains in the B dimension.</param>
        /// <param name="cCapacity">The maximum number of points this distribution contains in the C dimension.</param>
        UniformDistribution(int aCapacity=UnivariateDistribution::UnboundedCapacity, 
            int bCapacity=UnivariateDistribution::UnboundedCapacity, 
            int cCapacity=UnivariateDistribution::UnboundedCapacity);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="distribution">The distribution to copy.</param>
        UniformDistribution(const UniformDistribution& distribution);
        
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~UniformDistribution();

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
    /// Type safe convenience class for constructing uniform distribution enumerators.
    /// </summary>
    class UniformDistributionEnumerator : public DistributionEnumerator {
    public:
        /// <summary>
        ///  Construct a new enumerator for the uniform distribution.
        /// </summary>
        /// <param name="distribution">The uniform distribution for which an enumerator is required.</param>
        UniformDistributionEnumerator(UniformDistribution& distribution);        
    };
}
