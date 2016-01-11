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
#include "ResolutionModes.h"
#include "Pad.h"

namespace Plato {

    const ClassTypeInfo* Pad::TypeInfo = NULL;

    void Pad::Initializer() {
        TypeInfo = new ClassTypeInfo("Pad",300,1,NULL,Property::TypeInfo);
    }

    void Pad::Finalizer() {
        delete TypeInfo;
    }

   Pad::Pad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope) 
        : Property(creator,&parent,identity,scope) {
    }
    
    Pad::~Pad() {
    }
}
