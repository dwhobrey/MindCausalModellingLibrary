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

namespace Plato {

    FilterArguments::FilterArguments(Container* creator, Container* parent, Container* source, 
                   const Property* requester, PropertyModesEnum mode,Arguments* additionalArguments) {
        Creator = creator;
        Parent = parent;
        Source = source;
        Requester = requester;
        Mode = mode;
        AdditionalArguments = additionalArguments;
    }

    FilterArguments::~FilterArguments() {
        // Do not delete AdditionalArguments: they are shared with other filters and are deleted by PathNode.
        // delete AdditionalArguments;
    }

    const ClassTypeInfo* Filter::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Filter::PropertyTypes = NULL;

    void Filter::Initializer() {
        TypeInfo = new ClassTypeInfo("Filter",100,1,(ClassTypeInfo::FactoryDelegate)Filter::Factory,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(Property::TypeInfo,NULL);
    }

    void Filter::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Filter* Filter::Factory(FilterArguments* arguments) {
        return NULL;
    }

    Filter::Filter(Identifier& identifier, FilterArguments& arguments) 
        : Container(*PropertyTypes,arguments.Creator, arguments.Parent, identifier, PropertyScopes::Public) {
        Arguments = &arguments;
        Flags.IsFilter = true;
        Flags.Refresh = true;
        mDoDelete = false;
    }

    Filter::~Filter() {
        delete Arguments;
    }

    void Filter::Append(Filter* filter) {
        if (filter != NULL) {
            filter->Arguments->Source = Arguments->Source;
            Arguments->Source = filter;
        }
    }
}
