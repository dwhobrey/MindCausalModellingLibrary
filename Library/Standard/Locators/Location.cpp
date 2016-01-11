#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Point.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "ThreadLocalVariable.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Location.h"

namespace Plato {

    Location::Location(Locator& locatorPosition) {
        LocatorPosition = &locatorPosition;
        PointPosition = NULL;
        IsRegex = false;
    }

    Location::Location(Point& pointPosition) {
        LocatorPosition = NULL;
        PointPosition = &pointPosition;
        IsRegex = false;
    }

    int Location::CompareTo(Location& location) {
        // DEBUG: TO DO.
        return 0;
    }
}
