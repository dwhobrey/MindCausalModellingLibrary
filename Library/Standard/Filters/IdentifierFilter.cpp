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
#include "IdentifierFilter.h"
#include "IdentifierEnumerator.h"

namespace Plato {

    IdentifierFilterArguments::IdentifierFilterArguments(IdentifierRegex& comparisonRegex, bool includeWhenMatches) {
        IncludeWhenMatches = includeWhenMatches;
        ComparisonRegex = &comparisonRegex;
    }

    IdentifierFilterArguments::IdentifierFilterArguments(const IdentifierFilterArguments& arguments) {
        IncludeWhenMatches = arguments.IncludeWhenMatches;
        ComparisonRegex = new IdentifierRegex(*(arguments.ComparisonRegex));
    }

    IdentifierFilterArguments::~IdentifierFilterArguments() {
        delete ComparisonRegex;
    }

    Arguments* IdentifierFilterArguments::Clone() const {
        return new IdentifierFilterArguments(*this);
    }

    const ClassTypeInfo* IdentifierFilter::TypeInfo = NULL;

    void IdentifierFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("IdentifierFilter",102,1,(ClassTypeInfo::FactoryDelegate)IdentifierFilter::Factory,Filter::TypeInfo);
    }

    void IdentifierFilter::Finalizer() {
        delete TypeInfo;
    }

    IdentifierFilter::IdentifierFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("IdentifierFilter",NULL,NULL),arguments) {
    }

    IdentifierFilter::~IdentifierFilter() {
    }

    void IdentifierFilter::RefreshContents() {
        Flags.Refresh = false;
        IdentifierFilterArguments* ifa = (IdentifierFilterArguments*)(Arguments->AdditionalArguments);
        IdentifierRegex& regex = *(ifa->ComparisonRegex);
        const string* typeName = regex.TypeName;
        const ClassTypeInfo* type = (typeName==NULL || Regexes::IsRegex(*typeName)) ? NULL : ClassCatalogue::FindEntry(*typeName);
        if(type==NULL) {
            type = Property::TypeInfo; // If NULL, master table is searched.
        }
        IdentifierEnumerator* e = Arguments->Source->GetProperties(regex, *type, ifa->IncludeWhenMatches, Arguments->Requester, Arguments->Mode);
        if(e!=NULL) {
            while (e->MoveNext()) {
                Add(*(e->Current));
            }
            delete e;
        }
    }
    
    IdentifierFilter* IdentifierFilter::Factory(FilterArguments* arguments) {
        return new IdentifierFilter(*arguments);
    }
}
