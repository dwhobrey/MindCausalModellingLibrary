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
#include "BoxGeometry.h"

namespace Plato {

    BoxGeometry::BoxGeometry(Bounds& bounds, Scale& scale, Point* orientation, Distribution& distribution) 
        : Geometry(bounds, scale, orientation, distribution) {
    }
}
