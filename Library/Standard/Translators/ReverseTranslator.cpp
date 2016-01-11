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
#include "ReverseTranslator.h"

namespace Plato {

    const ClassTypeInfo* ReverseTranslator::TypeInfo = NULL;

    void ReverseTranslator::Initializer() {
        TypeInfo = new ClassTypeInfo("ReverseTranslator",251,1,NULL,Translator::TypeInfo);
    }

    void ReverseTranslator::Finalizer() {
        delete TypeInfo;
    }

    ReverseTranslator::ReverseTranslator() {
    }
    
    ReverseTranslator::~ReverseTranslator() {
    }

    void ReverseTranslator::OrderGroup(Connector& connector, GroupContainer& toGroup, GroupContainer& fromGroup) {
        // DEBUG: TO DO.
    }
}
