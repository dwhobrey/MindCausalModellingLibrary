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
#include "TypeNameFilter.h"
#include "ScopeEnumerator.h"

namespace Plato {

    TypeNameFilterArguments::TypeNameFilterArguments(const string& comparisonRegex, bool includeWhenMatches) {
        ComparisonRegex = &comparisonRegex;
        IncludeWhenMatches = includeWhenMatches;
    }
    TypeNameFilterArguments::TypeNameFilterArguments(const TypeNameFilterArguments& arguments) {
        IncludeWhenMatches = arguments.IncludeWhenMatches;
        ComparisonRegex = new string(*(arguments.ComparisonRegex));
    }

    TypeNameFilterArguments::~TypeNameFilterArguments() {
        delete ComparisonRegex;
    }
    Arguments* TypeNameFilterArguments::Clone() const {
        return new TypeNameFilterArguments(*this);
    }

    const ClassTypeInfo* TypeNameFilter::TypeInfo = NULL;

    void TypeNameFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("TypeNameFilter",109,1,(ClassTypeInfo::FactoryDelegate)TypeNameFilter::Factory,Filter::TypeInfo);
    }

    void TypeNameFilter::Finalizer() {
        delete TypeInfo;
    }

    TypeNameFilter::TypeNameFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("TypeNameFilter",NULL,NULL),arguments) {
    }

    TypeNameFilter::~TypeNameFilter() {
    }

    void TypeNameFilter::RefreshContents() {
        Flags.Refresh = false;
        TypeNameFilterArguments* tfa = (TypeNameFilterArguments*)(Arguments->AdditionalArguments);
        RegexMatcher regex(tfa->ComparisonRegex);
        ScopeEnumerator* e = new ScopeEnumerator(*(Arguments->Source),Arguments->Requester);
        if(tfa->IncludeWhenMatches) {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if (regex.Matches(*(p->GetValue()->GetClassTypeInfo()->ClassName))) {
                    Add(*p);
                }
            }
        } else {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if (!regex.Matches(*(p->GetValue()->GetClassTypeInfo()->ClassName))) {
                   Add(*p);
                }
            }
        }
        delete e;
    }

    TypeNameFilter* TypeNameFilter::Factory(FilterArguments* arguments) {
        return new TypeNameFilter(*arguments);
    }
}
