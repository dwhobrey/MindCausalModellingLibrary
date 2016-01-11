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
#include "ConverterRestrictor.h"
#include "Relator.h"
#include "ProximityRelator.h"

namespace Plato {

    const ClassTypeInfo* ConverterRestrictor::TypeInfo = NULL;

    void ConverterRestrictor::Initializer() {
        TypeInfo = new ClassTypeInfo("ConverterRestrictor",230,1,NULL,Restrictor::TypeInfo);
    }

    void ConverterRestrictor::Finalizer() {
        delete TypeInfo;
    }

    ConverterRestrictor::ConverterRestrictor(IdentifierRegex& converterRegex, bool includeWhenMatches, PropertyScopesEnum scope, ProximityRelator& relator)
            : Restrictor(converterRegex, includeWhenMatches, scope) {
        ElementRelator = &relator;
    }

    /// <summary>
    /// Deallocate object.
    /// </summary>
    ConverterRestrictor::~ConverterRestrictor() {
        delete ElementRelator;
    }

    bool ConverterRestrictor::IsDenied(ResolutionModesEnum resolutionMode, Container& container, vector<Property*>& elements) {
        return ElementRelator->MatchAndValidRelation(elements);
    }

    bool ConverterRestrictor::IsAllowed(ResolutionModesEnum resolutionMode, Container& container, vector<Property*>& elements) {
        vector<Property*>* restrictors = container.GetCategory(*Restrictor::TypeInfo);
        if (restrictors != NULL) {
            vector<Property*>::iterator itr;
            for(itr=restrictors->begin();itr!=restrictors->end();++itr) {
                Property* p = (*itr)->GetValue();
                if(p->Flags.IsRestrictor) {
                    if(p->GetClassTypeInfo()->TypeCode == TypeInfo->TypeCode) {
                        ConverterRestrictor* r = (ConverterRestrictor*)p;
                        if (r->IsDenied(resolutionMode, container, elements)) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
}
