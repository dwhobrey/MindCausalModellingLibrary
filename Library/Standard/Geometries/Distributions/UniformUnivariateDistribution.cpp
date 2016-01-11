#include "PlatoIncludes.h"
#include "Numbers.h"
#include "UnivariateDistribution.h"
#include "UnivariateDistributionEnumerator.h"
#include "UniformUnivariateDistribution.h"

namespace Plato {

    UniformUnivariateDistribution::UniformUnivariateDistribution(int capacity) 
        : UnivariateDistribution(capacity) {    
    }

    UniformUnivariateDistribution::UniformUnivariateDistribution(const UniformUnivariateDistribution& distribution) {
        mCapacity = distribution.mCapacity;
    }

    UniformUnivariateDistribution::~UniformUnivariateDistribution() {
    }

    UnivariateDistributionEnumerator* UniformUnivariateDistribution::GetEnumerator() {
        return new UniformUnivariateDistributionEnumerator(*this);
    }

    void UniformUnivariateDistribution::SetCapacity(int value) {
        UnivariateDistribution::SetCapacity(value);
        mDivisor = (float)((mCapacity == UnivariateDistribution::UnboundedCapacity ? UnivariateDistribution::MaximumCapacity : mCapacity) - 1.0f);
    }
    UnivariateDistribution* UniformUnivariateDistribution::Clone() const {
        return new UniformUnivariateDistribution(*this);
    }

    float UniformUnivariateDistribution::operator[](int index) {
        if (index < 0 || index > mDivisor) {
            return Numbers::OutOfRangeValue;
        }
        if (mDivisor == 0) {
            return 0.0f;
        }
        return (float)((float)(index) / (float)mDivisor);
    }

    UniformUnivariateDistributionEnumerator::UniformUnivariateDistributionEnumerator(UniformUnivariateDistribution& distribution) 
        : UnivariateDistributionEnumerator(distribution) {
    }
}
