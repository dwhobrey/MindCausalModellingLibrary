#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Identifier.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "Path.h"
#include "Link.h"

namespace Plato {

    const ClassTypeInfo* Link::TypeInfo = NULL;

    void Link::Initializer() {
        TypeInfo = new ClassTypeInfo("Link",5,1,NULL,Container::TypeInfo);
    }

    void Link::Finalizer() {
        delete TypeInfo;
    }

    Link::Link(Container* creator, Container* parent, Identifier& linkName, PropertyScopesEnum linkScope, 
            const Path& linkDestination) 
            : Container(*Container::PropertyTypes, creator, parent,linkName, linkScope) {
        mLinkDestination = &linkDestination;
        mDestinationValue = NULL; // Be lazy: evaluate on demand.
        Flags.IsProxied = true;
    }

    Link::~Link() {
        delete mLinkDestination;
    }

    Property* Link::GetProxy() {
        // Map to actual value.
        if (mDestinationValue == NULL) {
            Property* p = mLinkDestination->GetProperty(Creator,NULL,PropertyModes::Traversing);
            if (p != NULL) {
                mDestinationValue = p->GetValue();
            } else {
                return Property::NullProperty;
            }
        }
        return mDestinationValue;
    }
}
