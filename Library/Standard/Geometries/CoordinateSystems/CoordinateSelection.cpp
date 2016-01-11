#include "PlatoIncludes.h"
#include "CoordinateSelection.h"

namespace Plato {

    CoordinateSelection::CoordinateSelection(bool c0, bool c1, bool c2) {
        Selections[0] = c0;
        Selections[1] = c1;
        Selections[2] = c2;
    }
}
