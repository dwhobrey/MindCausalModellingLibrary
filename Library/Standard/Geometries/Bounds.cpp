#include "PlatoIncludes.h"
#include "Point.h"
#include "Size.h"
#include "Bounds.h"
#include "CoordinateOperations.h"
#include "CoordinateNames.h"
#include "Geometry.h"

namespace Plato {

    Bounds::Bounds(Point& lowerBound, Size& width) {
        Width = &width;
        LowerBound = &lowerBound;
    }

    Bounds::Bounds(Size& width) {
        Width = &width;
        LowerBound = new Point(*Geometry::ZeroPoint); // DEBUG: Should move ZeroPoint to Point class.
    }

    Bounds::Bounds(const Bounds& bounds) {
        Width = new Size(*bounds.Width);
        LowerBound = new Point(*bounds.LowerBound);
    }

    Bounds::~Bounds() {
        delete Width;
        delete LowerBound;
    }
}
