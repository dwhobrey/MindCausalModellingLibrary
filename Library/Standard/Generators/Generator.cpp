#include "PlatoIncludes.h"
#include "Generator.h"

namespace Plato {

    Generator::Generator(int capacity) {
        Capacity = capacity;
    }

    Generator::~Generator() {
    }

    Property* Generator::GetProperty(Container& ownerContainer, const Identifier& identifier) {
        // Derived class would create a new property at this point.
        // It may have to set the Name, Type, Position, and geometry of the property
        // and cater or adjust for any position dependent connections it may need.
        return NULL;
    }

    Property* Generator::GetProperty(Container& ownerContainer, Point& position) {
        // Derived class would create a new property at this point.
        // It will have to set the Name, Type, and geometry of the property
        // and cater or adjust for any position dependent connections it may need.
        return NULL;
    }
}
