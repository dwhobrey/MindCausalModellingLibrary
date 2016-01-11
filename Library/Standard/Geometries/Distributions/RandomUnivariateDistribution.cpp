#include "PlatoIncludes.h"
#include "Numbers.h"
#include "Random.h"
#include "UnivariateDistribution.h"
#include "UnivariateDistributionEnumerator.h"
#include "RandomUnivariateDistribution.h"

namespace Plato {

    RandomUnivariateDistribution::RandomUnivariateDistribution(int capacity) 
        : UnivariateDistribution(capacity) {
        mDistributionValues = NULL;
        Reset();
    }

    RandomUnivariateDistribution::RandomUnivariateDistribution(const RandomUnivariateDistribution& distribution) {
        mCapacity = distribution.mCapacity;
        mDistributionValues = NULL;
        // DEBUG: To do: Copy mDistributionValues.
    }

    RandomUnivariateDistribution::~RandomUnivariateDistribution() {
        delete mDistributionValues;
    }

    UnivariateDistributionEnumerator* RandomUnivariateDistribution::GetEnumerator() {
        return new RandomUnivariateDistributionEnumerator(*this);
    }

    void RandomUnivariateDistribution::SetCapacity(int value) {
        mCapacity = value;
        Reset();
    }
    
    void RandomUnivariateDistribution::Reset() {
        // Generate a sorted list of random numbers in [0.0,1.0], with no duplicates.
        if(mDistributionValues!=NULL) {
            mDistributionValues->clear();
        } else {
            mDistributionValues = new vector<float>(mCapacity);
        }
        Random& rnd = *new Random();
        float maxValue = (1.0f + (float)mCapacity) * 1000.0f;
        float divisor = maxValue - 1.0f;
        int index = 0;
        while ((int)mDistributionValues->size() < mCapacity) {
            float r = (float)((rnd.NextDouble() * maxValue) / divisor); // Map r to [0.0,1.0].
            if (r > UnivariateDistribution::MaximumX) {
                r = UnivariateDistribution::MaximumX;
            }
            (*mDistributionValues)[index++] = r;
        }
    }

    UnivariateDistribution* RandomUnivariateDistribution::Clone() const {
        return new RandomUnivariateDistribution(*this);
    }

    float RandomUnivariateDistribution::operator[](int index) {
        if (index < 0 || index >= (int)mDistributionValues->size()) {
            return Numbers::OutOfRangeValue;
        }
        return (*mDistributionValues)[index];
    }

    RandomUnivariateDistributionEnumerator::RandomUnivariateDistributionEnumerator(RandomUnivariateDistribution& distribution)
        : UnivariateDistributionEnumerator(distribution) {
        Reset();
    }

    RandomUnivariateDistributionEnumerator::~RandomUnivariateDistributionEnumerator() {
    }
}
