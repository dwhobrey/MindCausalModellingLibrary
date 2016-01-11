#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "ScopeEnumerator.h"

namespace Plato {

    ScopeEnumerator::ScopeEnumerator(Container& container, const Property* requester) {
        mSourceContainer = &container;
        mRequester = requester;
        Reset();
    }

    ScopeEnumerator::~ScopeEnumerator() {

    }

    void ScopeEnumerator::Reset() {
        Current = NULL;
        mNextPosition = mSourceContainer->OrderedProperties->begin();
        mEndPosition = mSourceContainer->OrderedProperties->end();
   }

    bool ScopeEnumerator::MoveNext() {
        while (mNextPosition!=mEndPosition) {
            Current = *mNextPosition++;
            if (mRequester == NULL) {
                return true;
            }
            if (Current != NULL) {
                // Check scope of item.
                if (Current==mRequester) {
                    PropertyScopesEnum scope = Current->Scope;
                    if (scope != PropertyScopes::External && scope != PropertyScopes::Unrelated) {
                        return true;
                    }
                } else if (Current->Parent==mRequester->Parent) {
                    PropertyScopesEnum scope = Current->Scope;
                    if (scope != PropertyScopes::Unrelated && scope != PropertyScopes::Self) {
                        return true;
                    }
                } else if (Current->InScope(mRequester, false)) {
                    return true;
                }
            } else {
                return true;
            }
        }
        Current = NULL;
        return false;
    }
}
