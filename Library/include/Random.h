#pragma once

namespace Plato {

    /// <summary>
    /// Class for generating pseudo random numbers.
    /// Based on combining three cyclic generators to give a long repeat epoch.
    /// </summary>
    class Random {

    private:
        int mInitialSeed;
        long mASeed;
        long mBSeed;
        long mCSeed;

    public:
        /// <summary>
        /// Construct a new generator.
        /// </summary>
        /// <param name="seed">Initialise random seed (if seed>0).</param>
        Random(int seed);

        /// <summary>
        /// Default constructor.
        /// The initial seed is set to zero.
        /// </summary>
        Random();

        /// <summary>
        /// Reset the generator to the start of the sequence.
        /// </summary>
        void Reset();

        /// <summary>
        /// Reseed generator.
        /// </summary>
        /// <param name="seed"></param>
        void Reseed(int seed);

        /// <summary>
        ///  Return a random number in [0.0 &lt;= r &lt; 1.0).
        /// </summary>
        double NextDouble();

        /// <summary>
        ///  Return random integer, [0 &lt;= r &lt; maxValue).
        /// </summary>
        /// <param name="maxValue">Upper bound, i.e. one more than maximum number to return.</param>
        /// <returns>Returns a pseudo random integer from zero up to, but not including maxValue.</returns>
        int Next(int maxValue);
    };
}
