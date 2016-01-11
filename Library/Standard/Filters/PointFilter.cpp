#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "ClassCatalogue.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "Container.h"
#include "Arguments.h"
#include "Filter.h"
#include "PointSpace.h"
#include "PointRegex.h"
#include "PointFilter.h"
#include "ScopeEnumerator.h"

namespace Plato {

    PointFilterArguments::PointFilterArguments(PointRegex& regexPoint, bool includeWhenMatches) {
        IncludeWhenMatches = includeWhenMatches;
        RegexPoint = &regexPoint;
    }
    PointFilterArguments::PointFilterArguments(const PointFilterArguments& arguments) {
        IncludeWhenMatches = arguments.IncludeWhenMatches;
        RegexPoint = new PointRegex(*(arguments.RegexPoint));
    }

    PointFilterArguments::~PointFilterArguments() {
        delete RegexPoint;
    }
    Arguments* PointFilterArguments::Clone() const {
        return new PointFilterArguments(*this);
    }

    const ClassTypeInfo* PointFilter::TypeInfo = NULL;

    void PointFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("PointFilter",106,1,(ClassTypeInfo::FactoryDelegate)PointFilter::Factory,Filter::TypeInfo);
    }

    void PointFilter::Finalizer() {
        delete TypeInfo;
    }

    PointFilter::PointFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("PointFilter",NULL,NULL),arguments) {
    }

    PointFilter::~PointFilter() {
    }

    void PointFilter::RefreshContents() {
        Flags.Refresh = false;
        PointFilterArguments* pfa = (PointFilterArguments*)(Arguments->AdditionalArguments);
        PointRegex* regexPoint = pfa->RegexPoint;
        ScopeEnumerator* e = new ScopeEnumerator(*(Arguments->Source),Arguments->Requester);
        if(pfa->IncludeWhenMatches) {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if (regexPoint->Match(*p->Identity->Position)) {
                    Add(*p);
                }
            }
        } else {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if (!regexPoint->Match(*p->Identity->Position)) {
                   Add(*p);
                }
            }
        }
        delete e;
    }

    
    PointFilter* PointFilter::Factory(FilterArguments* arguments) {
        return new PointFilter(*arguments);
    }
}
