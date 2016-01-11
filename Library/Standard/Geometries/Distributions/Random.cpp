#include "PlatoIncludes.h"
#include "Random.h"

namespace Plato {

    Random::Random(int seed) {
        Reseed(seed);
    }

    Random::Random() {
        Reseed(0);
    }

    void Random::Reset() {
        Reseed(mInitialSeed);
    }

    void Random::Reseed(int seed) {
        if(seed<1) {
            seed = 30013;
        }
        mInitialSeed = seed;
        mASeed=171*(seed%177);
        mBSeed=172*(seed%176);
        mCSeed=170*(seed%178);
    }

    double Random::NextDouble() {
        mASeed=171*(mASeed % 177)-2*(mASeed/177);
        if(mASeed<0) {
            mASeed+=30269;
        }
        mBSeed=172*(mBSeed % 176)-35*(mBSeed/176);
        if(mBSeed<0) {
            mBSeed+=30307;
        }
        mCSeed=170*(mCSeed % 178)-63*(mCSeed/178);
        if(mCSeed<0) {
            mCSeed+=30323;
        }
        double temp= mASeed/30269.0 + mBSeed/30307.0+mCSeed/30323.0;
        return temp - floor(temp);
    }

    int Random::Next(int maxValue) {
        if(maxValue<1) {
            return 0;
        }
        return (int)(maxValue*NextDouble());
    }
}
