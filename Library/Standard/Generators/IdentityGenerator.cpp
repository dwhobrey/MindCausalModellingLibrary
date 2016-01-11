#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "Generator.h"
#include "IdentityGenerator.h"

namespace Plato {

    IdentityGenerator::IdentityGenerator(Property* identityProperty) 
        : Generator(-1) {
        IdentityProperty = identityProperty;
    }

    Property* IdentityGenerator::GetProperty(Container& ownerContainer, const Identifier& identifier) {
        // DEBUG: Should this return a copy or the original?
        return IdentityProperty;
    }
}
