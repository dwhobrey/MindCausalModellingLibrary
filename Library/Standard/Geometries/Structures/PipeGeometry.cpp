#include "PlatoIncludes.h"
#include "Numbers.h"
#include "CoordinateNames.h"
#include "CoordinateOperations.h"
#include "Point.h"
#include "Bounds.h"
#include "Size.h"
#include "Scale.h"
#include "UnivariateDistribution.h"
#include "Distribution.h"
#include "DistributionEnumerator.h"
#include "UniformDistribution.h"
#include "PointEnumerator.h"
#include "Geometry.h"
#include "PipeGeometry.h"

namespace Plato {

    PipeGeometry::PipeGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution) 
        : Geometry(bounds, scale, orientation, distribution) {
        Reset();
    }

    PipeGeometry::PipeGeometry(int capacityX) 
        : Geometry(*new Bounds(*new Size((float)capacityX, 1.0f, 1.0f, Numbers::UnboundedValue)), 
        *new Scale(*Scale::Unity), NULL, *new UniformDistribution()) {
        Reset();
    }

    PipeGeometry::PipeGeometry(float capacityX) 
        : Geometry(*new Bounds(*new Size(capacityX, 1.0f, 1.0f, Numbers::UnboundedValue)), 
        * new Scale(*Scale::Unity), NULL, *new UniformDistribution()) {
        Reset();
    }

    PipeGeometry::~PipeGeometry() {
        delete mDX;
        delete mEX;
    }

    void PipeGeometry::Reset() {
        mDX = PointDistribution->Clone();
        mDX->SetCapacity((int)(InternalBounds->Width->Sizes[CoordinateNames::X]));
        mEX = NULL;
    }

    Point* PipeGeometry::GetPoint(CoordinateOperationsEnum operation, const PointSpace& pointSpace) {
        switch(operation) {
            case CoordinateOperations::New:
                // Fetch the next new point.
                // Uses one enumerator, but could use a counter via the distribution indexer.
                // Returns a new point or NULL if full.
                if (mEX == NULL) {
                    mEX = mDX->GetEnumerator();
                }

                if (!mEX->MoveNext()) {
                    return NULL;
                }

                return new Point( // DEBUG: Sort out.
                    0.0f, // (float)(mEX->Current) * mBounds->Size.WX, 
                    0.0f, 0.0f, 0.0f);
            default:
                break;
        }
        return NULL;
    }
}
