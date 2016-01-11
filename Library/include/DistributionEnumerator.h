#pragma once

namespace Plato {

    class Distribution;
    class Point;

    /// <summary>
    /// Represents an enumerator for a distribution.
    /// Ideally, it should be lazy.
    /// </summary>
    class DistributionEnumerator {
    protected:
        /// <summary>
        /// Indicates whether the enumerator is reset, i.e. one position before the start of the distribution sequence.
        /// </summary>
        bool mIsReset;

        /// <summary>
        /// Tracks the current position in the distribution sequence, -1 = reset position, 0 = first position, 1 = next, etc.
        /// </summary>
        int mCurrentPosition;

        /// <summary>
        /// The underlying distribution object whose distribution sequence is being enumerated.
        /// </summary>
        Distribution* mDistribution;

    public:
        /// <summary>
        /// Cache of the current distribution value.
        /// </summary>
        Point* Current;
        
        /// <summary>
        /// Construct a new enumerator on the specified distribution.
        /// </summary>
        /// <param name="distribution">The distribution whose sequence of points is to be enumerated over.</param>
        DistributionEnumerator(Distribution& distribution);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~DistributionEnumerator();
        
        /// <summary>
        /// Move to the next position in the distribution if possible.
        /// </summary>
        /// <returns>Returns true if there are more positions in the distribution. </returns>
        virtual bool MoveNext();

        /// <summary>
        /// Returns the enumerator to the start ready for re-use.
        /// </summary>
        virtual void Reset();
    };
}
