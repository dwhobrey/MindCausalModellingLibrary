#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "GroupContainer.h"
#include "Translator.h"

namespace Plato {

    const ClassTypeInfo* Translator::TypeInfo = NULL;

    void Translator::Initializer() {
        TypeInfo = new ClassTypeInfo("Translator",252,1,NULL,Property::TypeInfo);
    }

    void Translator::Finalizer() {
        delete TypeInfo;
    }

    Translator::Translator(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Property(creator, parent, elementName, scope) {
    }

    Translator::Translator() 
        : Property(NULL, NULL, Identifier::Generate("Translator"), PropertyScopes::Private) {
    }

    Translator::~Translator() {
    }

    void Translator::OrderGroup(Connector& connector, GroupContainer& toGroup, GroupContainer& fromGroup) {
    }
}
