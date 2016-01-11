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
#include "VacantFilter.h"

namespace Plato {

    VacantFilterArguments::VacantFilterArguments() {
    }
    VacantFilterArguments::VacantFilterArguments(const VacantFilterArguments& arguments) {
    }

    VacantFilterArguments::~VacantFilterArguments() {
    }
    Arguments* VacantFilterArguments::Clone() const {
        return new VacantFilterArguments(*this);
    }

    const ClassTypeInfo* VacantFilter::TypeInfo = NULL;

    void VacantFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("VacantFilter",110,1,(ClassTypeInfo::FactoryDelegate)VacantFilter::Factory,Filter::TypeInfo);
    }

    void VacantFilter::Finalizer() {
        delete TypeInfo;
    }

    VacantFilter::VacantFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("VacantFilter",NULL,NULL),arguments) {
    }

    VacantFilter::~VacantFilter() {
    }

    void VacantFilter::RefreshContents() {
        Flags.Refresh = false;
    }

    VacantFilter* VacantFilter::Factory(FilterArguments* arguments) {
        return new VacantFilter(*arguments);
    }
}
