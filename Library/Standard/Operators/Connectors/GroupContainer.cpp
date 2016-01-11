#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Identifier.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "PathNode.h"
#include "Path.h"
#include "GroupContainer.h"

namespace Plato {

    const ClassTypeInfo* GroupContainer::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* GroupContainer::PropertyTypes = NULL;

    void GroupContainer::Initializer() {
        TypeInfo = new ClassTypeInfo("GroupContainer",201,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(Property::TypeInfo,NULL);
    }

    void GroupContainer::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    GroupContainer::GroupContainer(Property* prop, GroupContainer* parentGroup, int groupNumber) 
        : Container(*PropertyTypes,prop->Parent, parentGroup, *prop->Identity, prop->Scope) {
        IsComplete = false;
        IsUnbounded = false;
        mDoDelete = false;
        GroupNumber = groupNumber;
        mValue = prop;
        Flags.IsProxied = true;
        Flags.IsGroupContainer = true;
    }

    GroupContainer::~GroupContainer() {
        Identity = NULL;
        vector<Property*>::iterator pitr;
        for(pitr=OrderedProperties->begin();pitr!=OrderedProperties->end();++pitr) {
            if((*pitr)->Flags.IsGroupContainer) {
                delete *pitr;
            }
        }
    }

    Property* GroupContainer::GetProxy() {
        return mValue;
    }

    GroupContainer* GroupContainer::Factory(Property* prop, PathNode* pathNode, GroupContainer* & treeRoot) {
        GroupContainer* current = new GroupContainer(prop, NULL, pathNode->GroupNumber);
        GroupContainer* last = current;
        treeRoot = current;
        int lastGroupNumber = pathNode->GroupNumber;
        // Ascend PathNode chain building group tree.
        while ((pathNode = pathNode->Parent) != NULL) {
            int num = pathNode->GroupNumber;
            if (prop != NULL) {
                prop = prop->Parent;
            }
            // Check if a more accurate property value is available.
            Property* tmp = pathNode->NodeProperty;
            if (tmp != NULL) {
                prop = tmp;
            }
            // Create new group if change in group numbers.
            if (num != lastGroupNumber) {
                lastGroupNumber = num;
                treeRoot = new GroupContainer(prop, NULL, num);
                last->Parent = treeRoot;
            }
        }
        return current;
    }

    GroupContainer* GroupContainer::AddGroup(Property* prop, int groupNumber) {
        GroupContainer* subTree = new GroupContainer(prop,this,groupNumber);
        Add(*subTree);
        return subTree;
    }

    void GroupContainer::AddProperty(Property* prop) {
        Add(*prop);
    }
}
