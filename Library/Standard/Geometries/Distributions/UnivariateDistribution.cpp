#include "PlatoIncludes.h"
#include "UnivariateDistribution.h"

namespace Plato {

    const float UnivariateDistribution::MinimumX = 0.0f;
    const float UnivariateDistribution::MaximumX = 1.0f;
    const float UnivariateDistribution::MinimumY = 0.0f;
    const float UnivariateDistribution::MaximumY = 1.0f;
    const int UnivariateDistribution::UnboundedCapacity = 0;
    const int UnivariateDistribution::MaximumCapacity = 10000000;

    UnivariateDistribution::UnivariateDistribution(int capacity) {
        mCapacity = capacity;
    }

    UnivariateDistribution::UnivariateDistribution(const UnivariateDistribution& distribution) {
        mCapacity = distribution.mCapacity;
    }

    UnivariateDistribution::~UnivariateDistribution() {
    }

    int UnivariateDistribution::Capacity() {
        return mCapacity;
    }

    void UnivariateDistribution::SetCapacity(int value) {
        mCapacity = (value <= 0) ? UnboundedCapacity : value;
    }

    UnivariateDistributionEnumerator* UnivariateDistribution::GetEnumerator() {
        return NULL;
    }

    UnivariateDistribution* UnivariateDistribution::Clone() const {
        return new UnivariateDistribution(*this);
    }

    float UnivariateDistribution::operator[](int index) {
        return 0.0f;
    }
}
