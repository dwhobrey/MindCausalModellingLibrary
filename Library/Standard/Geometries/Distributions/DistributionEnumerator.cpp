#include "PlatoIncludes.h"
#include "Numbers.h"
#include "UnivariateDistribution.h"
#include "CoordinateOperations.h"
#include "CoordinateNames.h"
#include "Distribution.h"
#include "DistributionEnumerator.h"
#include "Point.h"
#include "PointSpace.h"

namespace Plato {

    DistributionEnumerator::DistributionEnumerator(Distribution& distribution) {
        mDistribution = &distribution;
    }

    DistributionEnumerator::~DistributionEnumerator() {
    }

    bool DistributionEnumerator::MoveNext() {
        if (mIsReset) {
            mIsReset = false;
        }
        Current = mDistribution->GetPoint(CoordinateOperations::Next,NULL);
        return Current!=NULL && Current->Status!=PointStatus::OutOfRange;
    }

    void DistributionEnumerator::Reset() {
        Current = NULL;
        mCurrentPosition = -1;
        mIsReset = true;
    }
}
