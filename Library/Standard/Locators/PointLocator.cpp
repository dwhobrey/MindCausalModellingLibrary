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
#include "Locator.h"
#include "CoordinateNames.h"
#include "CoordinateOperations.h"
#include "PointLocator.h"
#include "Geometry.h"

namespace Plato {

    PointLocator::PointLocator(CoordinateOperationsEnum operation) {
        Operation = operation;
    }

    Point* PointLocator::ToPoint(Container& parentElement) {
        if (parentElement.ContainerGeometry == NULL) {
            return new Point();
        }
        return parentElement.ContainerGeometry->GetPoint(Operation,NULL);
    }
}
