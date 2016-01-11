#include "PlatoIncludes.h"
#include "Numbers.h"
#include "CoordinateOperations.h"
#include "CoordinateNames.h"
#include "Point.h"
#include "PointSpace.h"
#include "UnivariateDistribution.h"
#include "UnivariateDistributionEnumerator.h"
#include "NormalUnivariateDistribution.h"
#include "Distribution.h"
#include "DistributionEnumerator.h"
#include "NormalDistribution.h"

namespace Plato {

    NormalDistribution::NormalDistribution(int aCapacity, int bCapacity, int cCapacity, float mean, float sigma) {
        if(sigma==Numbers::UnboundedValue) {
            sigma = NormalUnivariateDistribution::NormalisedSigma(aCapacity);
        }
        mUnivariateDistributions[0] = new NormalUnivariateDistribution(aCapacity, mean, sigma);
        mUnivariateDistributions[1] = new NormalUnivariateDistribution(bCapacity, mean, sigma);
        mUnivariateDistributions[2] = new NormalUnivariateDistribution(cCapacity, mean, sigma);
    }

    NormalDistribution::NormalDistribution(const NormalDistribution& distribution) {
        mCapacity = distribution.mCapacity;
    }

    DistributionEnumerator* NormalDistribution::GetEnumerator() {
        return new NormalDistributionEnumerator(*this);
    }

    Distribution* NormalDistribution::Clone() const {
        return new NormalDistribution(*this);
    }

    NormalDistributionEnumerator::NormalDistributionEnumerator(NormalDistribution& distribution) 
            : DistributionEnumerator(distribution) {
        Reset();
    }
}
