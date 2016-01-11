#include "PlatoIncludes.h"
#include "Numbers.h"
#include "UnivariateDistribution.h"
#include "UnivariateDistributionEnumerator.h"
#include "NormalUnivariateDistribution.h"

namespace Plato {

    const double NormalUnivariateDistribution::NormPulseCoeff = 0.5 / sqrt(2.0 * log(2.0));
    const float NormalUnivariateDistribution::DefaultSigma = 0.5f;
    const float NormalUnivariateDistribution::DefaultMean = 0.5f;

    float NormalUnivariateDistribution::NormalisedSigma(int capacity) {
        return (float)(NormPulseCoeff / (float)(capacity == UnivariateDistribution::UnboundedCapacity ? 1 : capacity));
    }

    double NormalUnivariateDistribution::Gauss(float x) {
        double m = x - Mean;
        if (Sigma == 0.0f) {
            return 0.0;
        }
        return exp(mGaussCoeff * m * m );
    }

    double NormalUnivariateDistribution::InverseGauss(float y) {
        if (y <= 0.0f) {
            return 0.0;
        }
        return Sigma * sqrt(2.0 * log(1.0 / y));
    }

    NormalUnivariateDistribution::NormalUnivariateDistribution(int capacity, float mean, float sigma) 
            : UnivariateDistribution(capacity) {
        if (mean <= UnivariateDistribution::MinimumX || mean >= UnivariateDistribution::MaximumX) {
            mean = DefaultMean;
        }
        if (sigma <= UnivariateDistribution::MinimumX || sigma >= UnivariateDistribution::MaximumX) {
            sigma = DefaultMean;
        }
        Mean = mean;
        Sigma = sigma;
        mGaussCoeff = (float)(-0.5 / (Sigma * Sigma));
        YLow = (float)(Gauss(UnivariateDistribution::MinimumX));
        YHigh = (float)(Gauss(UnivariateDistribution::MaximumX));
    }

    NormalUnivariateDistribution::~NormalUnivariateDistribution() {
    }

    NormalUnivariateDistribution::NormalUnivariateDistribution(const NormalUnivariateDistribution& distribution) {
        mCapacity = distribution.mCapacity;
        Mean = distribution.Mean;
        Sigma = distribution.Sigma;
        mGaussCoeff = distribution.mGaussCoeff;
        YLow = distribution.YLow;
        YHigh = distribution.YHigh;
    }

    UnivariateDistributionEnumerator* NormalUnivariateDistribution::GetEnumerator() {
        return new NormalUnivariateDistributionEnumerator(*this);
    }

    void NormalUnivariateDistribution::SetCapacity(int value) {
        UnivariateDistribution::SetCapacity(value);
        mDivisor = (mCapacity == UnivariateDistribution::UnboundedCapacity ? UnivariateDistribution::MaximumCapacity : mCapacity - 1);
    }

    UnivariateDistribution* NormalUnivariateDistribution::Clone() const {
        return new NormalUnivariateDistribution(*this);
    }

    float NormalUnivariateDistribution::operator[](int index) {
        if (index < 0 || index > mDivisor) {
            return Numbers::OutOfRangeValue;
        }
        float y;
        float a;

        if (mDivisor == 0) {
            a = 0.0f;
        } else {
            a = (float)((float)(index) / (float)mDivisor);
        }
        if (a >= DefaultMean) {
            y = YHigh + (UnivariateDistribution::MaximumY - YHigh) * 2.0f * (UnivariateDistribution::MaximumX - a);
            return (float)(Mean + InverseGauss(y));
        }
        y = YLow + (UnivariateDistribution::MaximumY - YLow) * 2.0f * a;
        return (float)(Mean - InverseGauss(y));
    }

    NormalUnivariateDistributionEnumerator::NormalUnivariateDistributionEnumerator(NormalUnivariateDistribution& distribution)
        : UnivariateDistributionEnumerator(distribution) {
        Reset();
    }
}
