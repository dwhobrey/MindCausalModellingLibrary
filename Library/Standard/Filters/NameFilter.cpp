#include "PlatoIncludes.h"
#include "Strings.h"
#include "Regexes.h"
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
#include "NameFilter.h"
#include "ScopeEnumerator.h"

namespace Plato {

    NameFilterArguments::NameFilterArguments(string& comparisonRegex, bool includeWhenMatches) {
        IncludeWhenMatches = includeWhenMatches;
        ComparisonRegex = &comparisonRegex;
    }
    NameFilterArguments::NameFilterArguments(const NameFilterArguments& arguments) {
        IncludeWhenMatches = arguments.IncludeWhenMatches;
        ComparisonRegex = new string(*(arguments.ComparisonRegex));
    }

    NameFilterArguments::~NameFilterArguments() {
        delete ComparisonRegex;
    }
    Arguments* NameFilterArguments::Clone() const {
        return new NameFilterArguments(*this);
    }

    const ClassTypeInfo* NameFilter::TypeInfo = NULL;

    void NameFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("NameFilter",103,1,(ClassTypeInfo::FactoryDelegate)NameFilter::Factory,Filter::TypeInfo);
    }

    void NameFilter::Finalizer() {
        delete TypeInfo;
    }

    NameFilter::NameFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("NameFilter",NULL,NULL),arguments) {
    }

    NameFilter::~NameFilter() {
    }

    void NameFilter::RefreshContents() {
        Flags.Refresh = false;
        NameFilterArguments* nfa = (NameFilterArguments*)(Arguments->AdditionalArguments);
        string& regex = *(nfa->ComparisonRegex);
        RegexMatcher rm(&regex);
        ScopeEnumerator* e = new ScopeEnumerator(*(Arguments->Source),Arguments->Requester);
        if(nfa->IncludeWhenMatches) {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if (rm.Matches(*p->Identity->Name)) {
                    Add(*p);
                }
            }
        } else {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if (!rm.Matches(*p->Identity->Name)) {
                   Add(*p);
                }
            }
        }
        delete e;
    }

    NameFilter* NameFilter::Factory(FilterArguments* arguments) {
        return new NameFilter(*arguments);
    }
}
