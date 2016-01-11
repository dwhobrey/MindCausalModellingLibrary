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
#include "TypeFilter.h"

namespace Plato {

    TypeFilterArguments::TypeFilterArguments(const ClassTypeInfo& comparisonType, bool includeWhenMatches) {
        IncludeWhenMatches = includeWhenMatches;
        ComparisonType = &comparisonType;
    }
    TypeFilterArguments::TypeFilterArguments(const TypeFilterArguments& arguments) {
        IncludeWhenMatches = arguments.IncludeWhenMatches;
        ComparisonType = arguments.ComparisonType;
    }
    Arguments* TypeFilterArguments::Clone() const {
        return new TypeFilterArguments(*this);
    }

    const ClassTypeInfo* TypeFilter::TypeInfo = NULL;

    void TypeFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("TypeFilter",108,1,(ClassTypeInfo::FactoryDelegate)TypeFilter::Factory,Filter::TypeInfo);
    }

    void TypeFilter::Finalizer() {
        delete TypeInfo;
    }

    TypeFilter::TypeFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("TypeFilter",NULL,NULL),arguments) {
    }

    TypeFilter::~TypeFilter() {
    }

    void TypeFilter::RefreshContents() {
        Flags.Refresh = false;
        const ClassTypeInfo& type = *((TypeFilterArguments*)(Arguments->AdditionalArguments))->ComparisonType;
        vector<Property*>* v = Arguments->Source->GetCategory(type);
        if(v!=NULL) {
            vector<Property*>::iterator itr;
            for(itr=v->begin();itr!=v->end();++itr) {
                Add(*(*itr));
            }
        }
    }
   
    TypeFilter* TypeFilter::Factory(FilterArguments* arguments) {
        return new TypeFilter(*arguments);
    }
}
