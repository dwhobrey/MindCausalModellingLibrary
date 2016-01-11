#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Identifier.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "GroupContainer.h"
#include "GroupTreeEnumerator.h"
#include "PropertyEnumerator.h"

namespace Plato {

    GroupTreeEnumerator::GroupTreeEnumerator(GroupContainer& container, bool isTerminalGroup) {
        mContainer = &container;
        mIsTerminalGroup = isTerminalGroup;
        mEnumeratorList = new vector<PropertyEnumerator*>();
        mIsReset = true;
        Current = NULL; 
    }

    GroupTreeEnumerator::~GroupTreeEnumerator() {
        Reset();
        delete mEnumeratorList;
    }

    void GroupTreeEnumerator::Reset() {
        mIsReset = true;
        Current = NULL;
        vector<PropertyEnumerator*>::iterator itr;
        for(itr = mEnumeratorList->begin();itr!=mEnumeratorList->end();++itr) {
            delete (*itr);
        }
        mEnumeratorList->clear();
    }

    bool GroupTreeEnumerator::MoveNext() {
        if (mIsReset) {
            mIsReset = false;
            mEnumeratorList->push_back(new PropertyEnumerator(*(mContainer->OrderedProperties)));
            if (mIsTerminalGroup) {
                if (mContainer->GroupNumber == 0) {
                    Current = mContainer;
                    return true;
                }
            }
        }

        while (!mEnumeratorList->empty()) {
            PropertyEnumerator* pe = (*mEnumeratorList)[0];
            if (pe->MoveNext()) {
                Current = pe->Current; // If it's a GroupContainer do not go through GetValue().
                GroupContainer* container = Current->Flags.IsGroupContainer ? (GroupContainer*)Current : NULL;
                if (mIsTerminalGroup) {
                    if (container != NULL && container->GroupNumber == 0) {
                        return true;
                    }
                } else if (container != NULL) {
                    mEnumeratorList->push_back(new PropertyEnumerator(*(container->OrderedProperties)));
                } else {
                    return true;
                }
            } else {
                mEnumeratorList->erase(mEnumeratorList->begin());
            }
        }
        Current = NULL;
        return false;
    }
}
