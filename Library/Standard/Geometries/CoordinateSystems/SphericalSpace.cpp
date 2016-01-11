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
#include "SphericalSpace.h"

namespace Plato {

    const Point* SphericalSpace::UpperLimitPoint = NULL;
    const Point* SphericalSpace::LowerLimitPoint = NULL;
    const Point* SphericalSpace::ZeroPoint = NULL;

    void SphericalSpace::Initializer() {
        UpperLimitPoint = new Point(Numbers::MaxValue, Numbers::TwoPI, Numbers::PI, Numbers::MaxValue);
        LowerLimitPoint = new Point(0.0f, 0.0f, 0.0f, 0.0f);
        ZeroPoint = new Point(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void SphericalSpace::Finalizer() {
        delete UpperLimitPoint;
        delete LowerLimitPoint;
        delete ZeroPoint;
    }

    SphericalSpace::SphericalSpace(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution) 
        : Geometry(bounds, scale, orientation, distribution) {
    }
    
    const Point& SphericalSpace::GetUpperLimitPoint() {
        return *UpperLimitPoint;
    }

    const Point& SphericalSpace::GetLowerLimitPoint() {
        return *LowerLimitPoint;
    }

    Point& SphericalSpace::ToCartesian(Point& sphericalPoint) {
        float radius = sphericalPoint.Coordinates[0];
        double rst = radius * sin(sphericalPoint.Coordinates[2]);
        return *new Point(
            (float)(rst * cos(sphericalPoint.Coordinates[1])), 
            (float)(rst * sin(sphericalPoint.Coordinates[1])), 
            (float)(radius * sin(sphericalPoint.Coordinates[2])), 
            sphericalPoint.Coordinates[3]);
    }
}
