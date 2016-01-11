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
#include "SearchFilter.h"
#include "ScopeEnumerator.h"

namespace Plato {

    SearchFilterArguments::SearchFilterArguments(IdentifierRegex& regexIdentifier, bool includeWhenMatches, bool searchUp) {
        RegexIdentifier = &regexIdentifier;
        IncludeWhenMatches = includeWhenMatches;
        SearchUp = searchUp;
    }
    SearchFilterArguments::SearchFilterArguments(const SearchFilterArguments& arguments) {
        RegexIdentifier = new IdentifierRegex(*(arguments.RegexIdentifier));
        IncludeWhenMatches = arguments.IncludeWhenMatches;
        SearchUp = arguments.SearchUp;
    }

    SearchFilterArguments::~SearchFilterArguments() {
        delete RegexIdentifier;
    }
    Arguments* SearchFilterArguments::Clone() const {
        return new SearchFilterArguments(*this);
    }

    const ClassTypeInfo* SearchFilter::TypeInfo = NULL;

    void SearchFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("SearchFilter",107,1,(ClassTypeInfo::FactoryDelegate)SearchFilter::Factory,Filter::TypeInfo);
    }

    void SearchFilter::Finalizer() {
        delete TypeInfo;
    }

    SearchFilter::SearchFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("SearchFilter",NULL,NULL),arguments) {
    }

    void SearchFilter::RefreshContents() {
        Flags.Refresh = false;
        Search(Arguments->Source);
    }

    void SearchFilter::Search(Container* searchContainer) {
        SearchFilterArguments* sfa = (SearchFilterArguments*)(Arguments->AdditionalArguments);
        IdentifierRegex& regex = *(sfa->RegexIdentifier);
        bool searchUp = sfa->SearchUp;
        while (searchContainer != NULL) {
            ScopeEnumerator* e = new ScopeEnumerator(*searchContainer,Arguments->Requester);
            while (e->MoveNext()) {
                Property* p = e->Current;
                if (regex.Match(*p->Identity)) {
                    if (sfa->IncludeWhenMatches) {
                        Add(*p);
                    }
                } else {
                    if (!sfa->IncludeWhenMatches) {
                        Add(*p);
                    }
                }
                if (!searchUp) {
                    Property* c = p->GetValue();
                    if (c->Flags.IsContainer) {
                        Search((Container*)c);
                    }
                }
            }
            delete e;
            searchContainer = (searchUp ? searchContainer->Parent : NULL);
        }
    }

    SearchFilter* SearchFilter::Factory(FilterArguments* arguments) {
        return new SearchFilter(*arguments);
    }
}
