#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "ContainerEnumerator.h"

namespace Plato {

    ContainerEnumerator::ContainerEnumerator(Container* container, const Property* requester, 
        bool isWidthFirstDescent, bool includeContainers, bool includeLeaves, 
        bool includeLeafContainers, bool isReverse) {
        mContainer = container;
        mRequester = requester;
        mIsWidthFirstDescent = isWidthFirstDescent;
        mIncludeContainers = includeContainers;
        mIncludeLeaves = includeLeaves;
        mIncludeLeafContainers = includeLeafContainers;
        mIsReverse = isReverse;
        mEnumeratorList = new vector<ContainerIterator*>();
        mIsReset = true;
        Current = NULL; 
    }

    ContainerEnumerator::~ContainerEnumerator() {
        Reset();
        delete mEnumeratorList;
    }

    void ContainerEnumerator::Reset() {
        mIsReset = true;
        Current = NULL;
        vector<ContainerIterator*>::iterator itr;
        for(itr = mEnumeratorList->begin();itr!=mEnumeratorList->end();++itr) {
            delete *itr;
        }
        mEnumeratorList->clear();
    }

    bool ContainerEnumerator::MoveNext() {
        if (mIsReset) {
            mIsReset = false;
            if (mContainer != NULL) {
                ContainerIterator* ci = new ContainerIterator(mContainer->OrderedProperties,mIsReverse);
                mEnumeratorList->push_back(ci);
                if (mIncludeContainers) {
                    Current = mContainer;
                    return true;
                }
            }
        }

        int count;
        while ((count=(int)(mEnumeratorList->size())) != 0) {
            ContainerIterator* ci = (*mEnumeratorList)[mIsWidthFirstDescent ? 0 : count - 1];
        tryNext :
            if (ci->MoveNext()) {
                Current = ci->Current;
                if(Current->Flags.IsContainer) {
                    Container* container =(Container*)Current;
                    ContainerIterator* e = new ContainerIterator(container->OrderedProperties,mIsReverse);
                    if (mIsWidthFirstDescent) {
                        mEnumeratorList->push_back(e);
                    } else {
                        mEnumeratorList->insert(mEnumeratorList->begin(), e);
                    }
                    if (mIncludeContainers) {
                        return true;
                    }
                    if (container->Count() == 0 && mIncludeLeafContainers) {
                        return true;
                    }
                    continue;
                }
                if (mIncludeLeaves) {
                    return true;
                } else {
                    goto tryNext;
                }
            } else {
                mEnumeratorList->erase(mEnumeratorList->begin() + (mIsWidthFirstDescent ? 0 : --count));
                continue;
            }
        }
        Current = NULL;
        return false;
    }
}

