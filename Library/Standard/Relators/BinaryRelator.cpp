#include "PlatoIncludes.h"
#include "Strings.h"
#include "Numbers.h"
#include "ClassTypeInfo.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "Point.h"
#include "Relator.h"
#include "ProximityRelator.h"
#include "BinaryRelator.h"

namespace Plato {

    BinaryRelator::BinaryRelator(const Point& relativePosition, Identifier& elementNameA, Identifier& elementNameB)
        : ProximityRelator(NULL) {
        RelativePosition = &relativePosition;
        ElementNameA = &elementNameA;
        ElementNameB = &elementNameB;
    }

    const Point* NextXRelator::NextXPosition = NULL;

    void NextXRelator::Initializer() {
        NextXPosition = new Point(1.0f, Numbers::PI, (float)(Numbers::PI / 2.0f), 0.0f);
    }

    void NextXRelator::Finalizer() {
        delete NextXPosition;
    }

    NextXRelator::NextXRelator(Identifier& elementNameA, Identifier& elementNameB) 
        : BinaryRelator(*NextXPosition, elementNameA, elementNameB) {
    }
}
