#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "Arguments.h"
#include "CoordinateNames.h"
#include "CoordinateOperations.h"
#include "Point.h"
#include "Bounds.h"
#include "Size.h"
#include "Scale.h"
#include "UnivariateDistribution.h"
#include "Distribution.h"
#include "PointEnumerator.h"
#include "Geometry.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "Generator.h"
#include "ConstructorGenerator.h"

namespace Plato {

    ConstructorGenerator::ConstructorGenerator(ClassTypeInfo& propertyType, Identifier* propertyName, 
            Geometry* propertyGeometry, Arguments* constructorArguments)
            : Generator(-1) {
        PropertyType = &propertyType;
        PropertyName = propertyName;
        PropertyGeometry = propertyGeometry;
        ConstructorArguments = constructorArguments;
    }

    ConstructorGenerator::~ConstructorGenerator() {
        // DEBUG: Should these be deleted?
        delete PropertyName;
        delete PropertyGeometry;
        delete ConstructorArguments;
    }

    Property* ConstructorGenerator::GetProperty(Container& ownerContainer, const Identifier& identifier) {

        Property* prop = (*(PropertyType->Factory))(ConstructorArguments);

        if (prop == NULL) {
            return NULL; 
        }

        prop->Creator = &ownerContainer;

        if(prop->Flags.IsContainer) {
            Container* ge = (Container*)prop; // DEBUG: Don't go through GetValue()?
            if (ge != NULL) {
                // DEBUG: TO DO: Why are we just copying geometry?
                ge->ContainerGeometry = PropertyGeometry->Clone();
            }
        }

        // DEBUG: TO DO: Set other properties etc? Such as Identity?

        return prop;
    }
}
