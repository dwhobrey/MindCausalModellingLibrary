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
#include "CylindricalSpace.h"

namespace Plato {
    
    const Point* CylindricalSpace::UpperLimitPoint = NULL;
    const Point* CylindricalSpace::LowerLimitPoint = NULL;
    const Point* CylindricalSpace::ZeroPoint = NULL;

    void CylindricalSpace::Initializer() {
        UpperLimitPoint = new Point(Numbers::MaxValue, Numbers::TwoPI, Numbers::MaxValue, Numbers::MaxValue);
        LowerLimitPoint = new Point(0.0f, 0.0f, Numbers::MinValue, 0.0f);
        ZeroPoint = new Point(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void CylindricalSpace::Finalizer() {
        delete UpperLimitPoint;
        delete LowerLimitPoint;
        delete ZeroPoint;
    }

    CylindricalSpace::CylindricalSpace(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution) 
        : Geometry(bounds, scale, orientation, distribution) {
    }

    const Point& CylindricalSpace::GetUpperLimitPoint() {
        return *UpperLimitPoint;
    }

    const Point& CylindricalSpace::GetLowerLimitPoint() {
        return *LowerLimitPoint;
    }

    Point& CylindricalSpace::ToCartesian(Point& cylindricalPoint) {
        float radius = cylindricalPoint.Coordinates[0];
        float phi = cylindricalPoint.Coordinates[1];
        return *new Point(
            (float)(radius * cos(phi)), 
            (float)(radius * sin(phi)), 
            cylindricalPoint.Coordinates[2], 
            cylindricalPoint.Coordinates[3]);
    }
}
