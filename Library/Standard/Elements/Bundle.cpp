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
#include "Pad.h"
#include "InputPad.h"
#include "OutputPad.h"
#include "Bundle.h"

namespace Plato {

    const ClassTypeInfo* Bundle::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Bundle::PropertyTypes = NULL;

    void Bundle::Initializer() {
        TypeInfo = new ClassTypeInfo("Bundle",303,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(
            Bundle::TypeInfo, Pad::TypeInfo,
            NULL);
    }

    void Bundle::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Bundle::Bundle(Container* creator, Container& parent, Identifier& elementName, PropertyScopesEnum scope, bool isInput) 
        : Container(*PropertyTypes, creator, &parent, elementName, scope) {
        mIsInput = isInput;
    }

    Bundle::~Bundle() {
    }

    Property* Bundle::PropertyFactory(Identifier& identifier, const Property* requester) {
        Property* p;
        if (mIsInput)
            p = new InputPad(this, *this, identifier, PropertyScopes::Public);
        else
            p = new OutputPad(this, *this, identifier, PropertyScopes::Public);
        Add(*p);
        return p;
    }
}
