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
#include "UniformDistribution.h"

namespace Plato {

    UniformDistribution::UniformDistribution(int aCapacity, int bCapacity, int cCapacity) 
        : Distribution(aCapacity) {
        mCapacities[0] = aCapacity;
        mCapacities[1] = bCapacity;
        mCapacities[2] = cCapacity;
    }
    UniformDistribution::UniformDistribution(const UniformDistribution& distribution) {
        mCapacity = distribution.mCapacity;
    }
    
    UniformDistribution::~UniformDistribution() {
    }

    DistributionEnumerator* UniformDistribution::GetEnumerator() {
        return new UniformDistributionEnumerator(*this);
    }

    Distribution* UniformDistribution::Clone() const {
        return new UniformDistribution(*this);
    }

    UniformDistributionEnumerator::UniformDistributionEnumerator(UniformDistribution& distribution)
        : DistributionEnumerator(distribution) {
    }
}
