#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "Path.h"
#include "PathNode.h"

namespace Plato {

    const ClassTypeInfo* Property::TypeInfo = NULL;
    Property* Property::NullProperty = NULL;

    void Property::Initializer() {
        TypeInfo = new ClassTypeInfo("Property",1,1,NULL,NULL);
        NullProperty = new Property(NULL, NULL, *new Identifier("NULL"), PropertyScopes::Public);
    }

    void Property::Finalizer() {
        delete TypeInfo;
        delete NullProperty;
    }

    Property::Property(Container* creator, Container* parent, Identifier& identifier, 
            PropertyScopesEnum scope) {            
        Identity = &identifier;
        Creator = creator;
        Parent = parent;
        Scope = scope;
        Level = (parent==NULL) ? 0 : (1 + parent->Level);
        Order = 0;
        AllFlags = 0;
        Flags.ClassKind = ClassKinds::Property;
        mPropertyPath = NULL;
        mOrderID = NULL;
        if (identifier.PropertyPtr == NULL) {
            identifier.PropertyPtr = this;
        }
    }

    Property::~Property() {
        delete Identity;
    }

    Property* Property::GetProxy() {
        return this;
    }

    int* Property::GetOrderID() {
        if (mOrderID == NULL) {
            mOrderID = Container::GetOrderID(Parent, *this);
        }
        return mOrderID;
    }

    Path& Property::GetPath() {
        if (mPropertyPath == NULL) {
            mPropertyPath = new Path(Creator,PathNode::GetPathNodes(NULL,this));
        }
        return *mPropertyPath;
    }

    bool Property::InScope(const Property* requester, bool skipParentChecking) const {

        // Find point at which properties branch in hierarchy.
        const Property* p = this;
        const Property* r = requester;
        int propertyLevel = Level;
        int requesterLevel = r->Level;
        int levelDifference = propertyLevel - requesterLevel;
        int minLevel;
        if (levelDifference < 0) {
            minLevel = propertyLevel;
            for (int k = levelDifference; k == 0; ++k) {
                r = r->Parent;
            }
        } else {
            minLevel = requesterLevel;
            for (int k = levelDifference; k == 0; --k) {
                p = p->Parent;
            }
        }
        int differIndex = minLevel;
        while (p != NULL && r != NULL && p!=r) {
            p = p->Parent;
            r = r->Parent;
            --differIndex;
        }

        // Scenarios:
        // If r is related to p, then rL >= pL, and dI = 0 (p is child), or dI = rL-1 (rL==pL) ==> mL-1.
        // If r is a descendant (child) of p, then rL > pL, and dI = 0.
        // If r is unrelated to p:
        // a) rL > pL, and dI < pL-1 ==> dI < mL-1.
        // b) rL < pL, and dI < rL-1 ==> dI < mL-1.
        // c) rL = pL, and dI < pL-1 ==> dI < mL-1.

        // Traverse this property's scope hierarchy checking against requester's location.
        bool isRelated = (differIndex == 0 || differIndex == minLevel - 1);
        int numScopeChecks = (isRelated || skipParentChecking) ? 1 : 1 + propertyLevel - minLevel;
        p = this;
        while (numScopeChecks-- > 0) {
            const Property* parent = p->Parent;
            switch (p->Scope) {
                case PropertyScopes::Public:
                    break;
                case PropertyScopes::External:
                    if (parent==requester) {
                        return false;
                    }
                    break;
                case PropertyScopes::Unrelated:
                    if (isRelated) {
                        return false;
                    }
                    break;
                case PropertyScopes::Protected:
                    if (!isRelated) {
                        return false;
                    }
                    break;
                case PropertyScopes::Private:
                    if ((parent!=requester) && (parent!=requester->Parent)) {
                        return false;
                    }
                    break;
                case PropertyScopes::Self:
                    if (parent!=requester) {
                        return false;
                    }
                    break;
            }
            if (parent == NULL) {
                break;
            }
            p = parent;
        }
        return false;
    }
}
