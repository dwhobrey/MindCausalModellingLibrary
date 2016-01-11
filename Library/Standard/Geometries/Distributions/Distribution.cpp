#include "PlatoIncludes.h"
#include "Numbers.h"
#include "UnivariateDistribution.h"
#include "CoordinateOperations.h"
#include "CoordinateNames.h"
#include "Point.h"
#include "PointSpace.h"
#include "Distribution.h"

namespace Plato {

    Distribution::Distribution(int capacity) {
        mCapacity = capacity;
    }

    Distribution::Distribution(const Distribution& distribution) {
        mCapacity = distribution.mCapacity;
    }

    Distribution::~Distribution() {
    }

    int Distribution::Capacity() {
        return mCapacity;
    }

    void Distribution::SetCapacity(int value) {
        mCapacity = (value <= 0) ? UnivariateDistribution::UnboundedCapacity : value;
    }

    DistributionEnumerator* Distribution::GetEnumerator() {
        return NULL;
    }

    Distribution* Distribution::Clone() const {
        return new Distribution(*this);
    }

    Point* Distribution::GetPoint(CoordinateOperationsEnum operation, const PointSpace* pointSpace) {
        // DEBUG: TO DO.
        return NULL;
    }

    float Distribution::GetCoordinate(CoordinateOperationsEnum operation, CoordinateNamesEnum coordinate, const PointSpace& pointSpace) {
        // DEBUG: This could use GetPoint.
        return Numbers::OutOfRangeValue;
    }
}
