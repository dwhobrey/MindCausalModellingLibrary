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
#include "LayerGeometry.h"

namespace Plato {

    LayerGeometry::LayerGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution) 
        : Geometry(bounds, scale, orientation, distribution) {
        Reset();
    }

    LayerGeometry::LayerGeometry(int capacityX, int capacityY) 
        : Geometry(*new Bounds(*new Size((float)capacityX, (float)capacityY, 1.0f, Numbers::UnboundedValue)),
            *new Scale(*Scale::Unity), NULL, *new UniformDistribution()) {
        Reset();
    }

    void LayerGeometry::Reset() {
        mDX = PointDistribution->Clone();
        mDX->SetCapacity((int)(InternalBounds->Width->Sizes[CoordinateNames::X]));
        mEX = NULL;

        mDY = PointDistribution->Clone();
        mDY->SetCapacity((int)(InternalBounds->Width->Sizes[CoordinateNames::Y]));
        mEY = NULL;
    }

    Point* LayerGeometry::GetPoint(CoordinateOperationsEnum operation, const PointSpace& pointSpace) {
        switch (operation) {
            case CoordinateOperations::New:
                // Fetch the next new point.
                // Uses two enumerators, but could use counters via the distribution indexers.
                // Returns a new point or NULL if full.
                if (mEY == NULL) {
                    mEY = mDY->GetEnumerator();
                    if (!mEY->MoveNext()) {
                        return NULL;
                    }
                }

                if (mEX == NULL) {
                    mEX = mDX->GetEnumerator();
                }

                while (!mEX->MoveNext()) {
                    if (!mEY->MoveNext()) {
                        return NULL;
                    }
                    mEX = mDX->GetEnumerator();
                }

                return new Point( // DEBUG: Sort out.
                    0.0f, // (float)((float)(mEX->Current) * InternalBounds->Width->Sizes[CoordinateNames::X]),
                    0.0f, // (float)((float)(mEY->Current) * InternalBounds->Width->Sizes[CoordinateNames::Y]),
                    0.0f,
                    0.0f);
            default:
                break;
        }
        return NULL;
    }
}
