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
#include "Alias.h"

namespace Plato {

    const ClassTypeInfo* Alias::TypeInfo = NULL;

    void Alias::Initializer() {
        TypeInfo = new ClassTypeInfo("Alias",4,1,NULL,Property::TypeInfo);
    }

    void Alias::Finalizer() {
        delete TypeInfo;
    }

    Alias::Alias(Container* creator, Container* parent, Identifier& aliasName, PropertyScopesEnum aliasScope, 
        Property* aliasedProperty, bool isOwner) 
        : Property(creator,parent,aliasName,aliasScope) {
        mAliasedProperty = aliasedProperty;
        mCachedProperty = NULL;
        IsOwner = isOwner;
        IsPath = false;
        Flags.IsProxied = true;
    }

    Alias::Alias(Container* creator, Container* parent, Identifier& aliasName, PropertyScopesEnum aliasScope, 
        Path* aliasedPath) 
        : Property(creator,parent,aliasName,aliasScope) {
        mAliasedPath = aliasedPath;
        mCachedProperty = NULL;
        IsOwner = true;
        IsPath = true;
        Flags.IsProxied = true;
    }

    Alias::~Alias() {
        if(IsOwner) {
            if(IsPath) {
                delete mAliasedPath;
            } else {
                delete mAliasedProperty;
            }
        }
    }

    Property* Alias::GetProxy() {
        if(IsPath) {
            if (mAliasedPath != NULL) {
                Property* p = mAliasedPath->GetProperty(Creator,NULL,PropertyModes::Traversing);
                if (p != NULL) {
                    // Do not cache path value.
                    return p->GetValue();
                }
            }
        } else if (mCachedProperty == NULL && mAliasedProperty != NULL) {
            mCachedProperty = mAliasedProperty->GetValue();
        }
        return mCachedProperty == NULL ? Property::NullProperty : mCachedProperty;
    }
}
