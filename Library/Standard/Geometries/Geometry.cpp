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
#include "PointEnumerator.h"
#include "Geometry.h"

namespace Plato {

    const Point* Geometry::OutOfRangePoint = NULL;
    const Point* Geometry::UnboundedPoint = NULL;
    const Point* Geometry::UpperLimitPoint = NULL;
    const Point* Geometry::LowerLimitPoint = NULL; 
    const Point* Geometry::ZeroPoint = NULL;

    void Geometry::Initializer() {
        OutOfRangePoint = new Point(PointStatus::OutOfRange,Numbers::OutOfRangeValue, Numbers::OutOfRangeValue, Numbers::OutOfRangeValue, Numbers::OutOfRangeValue);
        UnboundedPoint = new Point(PointStatus::Unbounded,Numbers::UnboundedValue, Numbers::UnboundedValue, Numbers::UnboundedValue, Numbers::UnboundedValue);
        UpperLimitPoint = new Point(Numbers::MaxValue, Numbers::MaxValue, Numbers::MaxValue, Numbers::MaxValue);
        LowerLimitPoint = new Point(Numbers::MinValue, Numbers::MinValue, Numbers::MinValue, 0.0f);
        ZeroPoint = new Point(0.0f, 0.0f, 0.0f, 0.0f);    
    }

    void Geometry::Finalizer() {
        delete OutOfRangePoint;
        delete UnboundedPoint;
        delete UpperLimitPoint;
        delete LowerLimitPoint;
        delete ZeroPoint;
    }

    Geometry::Geometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution) {
        InternalBounds = &bounds;
        SpatialScale = &scale;
        Orientation = orientation;
        PointDistribution = &distribution;
        GeometryContainer = NULL; // This is set later.
    }

    Geometry::Geometry(const Geometry& geometry) {
        InternalBounds = new Bounds(*geometry.InternalBounds);
        SpatialScale = new Scale(*geometry.SpatialScale);
        Orientation = new Point(*geometry.Orientation);
        PointDistribution = geometry.PointDistribution->Clone();
        GeometryContainer = geometry.GeometryContainer;
    }

    Geometry::~Geometry() {
        delete InternalBounds;
        delete SpatialScale;
        delete Orientation;
        delete PointDistribution;
    }

    const Point& Geometry::GetUpperLimitPoint() {
        return *UpperLimitPoint;
    }

    const Point& Geometry::GetLowerLimitPoint() {
        return *LowerLimitPoint;
    }

    bool Geometry::IsOutOfRange(const Point& point) {
        int n = Point::NumberOfCoordinates;
        const float* upper = GetUpperLimitPoint().Coordinates;
        const float* lower = GetLowerLimitPoint().Coordinates;
        const float* coordinates = point.Coordinates;
        while (n-- > 0) {
            float val = coordinates[n];
            if (val < lower[n] || val > upper[n] || Numbers::IsSpecial(val)) {
                return true;
            }
        }
        return false;
    }

    Point* Geometry::GetPoint(CoordinateOperationsEnum operation, const PointSpace* pointSpace) {
        // DEBUG: TO DO.
        return NULL;
    }

    float Geometry::GetCoordinate(CoordinateOperationsEnum operation, CoordinateNamesEnum coordinate, const PointSpace* pointSpace) {
        // DEBUG: This could use GetPoint.
        return Numbers::OutOfRangeValue;
    }

    int Geometry::GetCapacity(const PointSpace& pointSpace) {
        return 0;
    }

    int Geometry::Count(const PointSpace& pointSpace) {
        return 0;
    }

    PointEnumerator* Geometry::GetEnumerator() {
        return NULL;
    }

    Geometry* Geometry::Clone() const {
        return new Geometry(*this);
    }
}
