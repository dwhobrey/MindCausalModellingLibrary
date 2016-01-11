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
#include "GenericTranslator.h"

namespace Plato {

    const ClassTypeInfo* GenericTranslator::TypeInfo = NULL;

    void GenericTranslator::Initializer() {
        TypeInfo = new ClassTypeInfo("GenericTranslator",250,1,NULL,Translator::TypeInfo);
    }

    void GenericTranslator::Finalizer() {
        delete TypeInfo;
    }

    GenericTranslator::GenericTranslator() {
    }

    GenericTranslator::~GenericTranslator() {
    }

    void GenericTranslator::OrderGroup(Connector& connector, GroupContainer& toGroup, GroupContainer& fromGroup) {
        // DEBUG: TO DO.
    }
}
