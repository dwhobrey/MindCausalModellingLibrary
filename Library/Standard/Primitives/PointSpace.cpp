#include "PlatoIncludes.h"
#include "Point.h"
#include "PointSpace.h"

namespace Plato {

    PointSpace::PointSpace() {
    }

    PointSpace::~PointSpace() {
    }

    bool PointSpace::IsContained(const Point& point) {
        return true;
    }

    float PointSpace::Volume() {
        return 1.0f;
    }

    Bounds* PointSpace::BoundingBox() {
        return NULL;
    }
}
