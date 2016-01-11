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
#include "Restrictor.h"

namespace Plato {

    const ClassTypeInfo* Restrictor::TypeInfo = NULL;

    void Restrictor::Initializer() {
        TypeInfo = new ClassTypeInfo("Restrictor",233,1,NULL,Property::TypeInfo);
    }

    void Restrictor::Finalizer() {
        delete TypeInfo;
    }

    Restrictor::Restrictor(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope,
                           IdentifierRegex& operatorRegex, bool includeWhenMatches, PropertyScopesEnum operatorScope) 
            : Property(creator, parent, elementName, scope) {
        IncludeWhenMatches = includeWhenMatches;
        OperatorScope = operatorScope;
        OperatorRegex = &operatorRegex;
        Flags.IsRestrictor = true;
    }

    Restrictor::Restrictor(IdentifierRegex& operatorRegex, bool includeWhenMatches, PropertyScopesEnum operatorScope) 
        : Property(NULL, NULL, Identifier::Generate("Restrictor"), PropertyScopes::Private) {
        IncludeWhenMatches = includeWhenMatches;
        OperatorScope = operatorScope;
        OperatorRegex = &operatorRegex;
        Flags.IsRestrictor = true;
    }

    Restrictor::~Restrictor() {
        delete OperatorRegex;
    }

    bool Restrictor::IsDenied(Property& configureOperator, ResolutionModesEnum resolutionMode, Container& container) {
        return false;
    }

    bool Restrictor::IsAllowed(Property& configureOperator, ResolutionModesEnum resolutionMode, Container& container) {
        vector<Property*> * restrictors = container.GetCategory(*Restrictor::TypeInfo);
        if (restrictors != NULL) {
            vector<Property*>::iterator itr;
            for(itr=restrictors->begin();itr!=restrictors->end();++itr) {
                Property* prop = (*itr)->GetValue();
                if(prop->Flags.IsRestrictor) {
                    Restrictor* r = (Restrictor*)prop;
                    if (r->IsDenied(configureOperator, resolutionMode, container)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
}
