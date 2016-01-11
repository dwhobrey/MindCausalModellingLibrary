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
#include "SphericalSpace.h"
#include "DiscGeometry.h"

namespace Plato {

    DiscGeometry::DiscGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution) 
        : SphericalSpace(bounds, scale, orientation, distribution) {
    }
}
