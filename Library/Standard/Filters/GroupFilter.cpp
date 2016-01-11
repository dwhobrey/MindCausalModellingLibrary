#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Identifier.h"
#include "Property.h"
#include "Container.h"
#include "Arguments.h"
#include "Filter.h"
#include "GroupFilter.h"

namespace Plato {

    const ClassTypeInfo* GroupFilter::TypeInfo = NULL;

    void GroupFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("GroupFilter",101,1,(ClassTypeInfo::FactoryDelegate)GroupFilter::Factory,Filter::TypeInfo);
    }

    void GroupFilter::Finalizer() {
        delete TypeInfo;
    }

    GroupFilter::GroupFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("GroupFilter",NULL,NULL),arguments) {
        Flags.IsGroupFilter = true;
    }

    void GroupFilter::RefreshContents() {
        Flags.Refresh = false;
        if (Arguments->Source != NULL) {
            Arguments->Source->CloneContents(*this);
            OrderGroup();
        }
    }

    void GroupFilter::OrderGroup() {
    }

    Filter* GroupFilter::Factory(FilterArguments* arguments) {
        return new GroupFilter(*arguments);
    }
}
