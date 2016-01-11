#include "PlatoIncludes.h"
#include "Numbers.h"
#include "UnivariateDistribution.h"
#include "UnivariateDistributionEnumerator.h"

namespace Plato {

    UnivariateDistributionEnumerator::UnivariateDistributionEnumerator(UnivariateDistribution& distribution) {
        mDistribution = &distribution;
        Reset();
    }

    UnivariateDistributionEnumerator::~UnivariateDistributionEnumerator() {
    }

    bool UnivariateDistributionEnumerator::MoveNext() {
        if (mIsReset) {
            mIsReset = false;
        }
        Current = (*mDistribution)[++mCurrentPosition];
        return Current != Numbers::OutOfRangeValue;
    }

    void UnivariateDistributionEnumerator::Reset() {
        Current = Numbers::OutOfRangeValue;
        mCurrentPosition = -1;
        mIsReset = true;
    }
}
