#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "IdentifierEnumerator.h"

namespace Plato {

    IdentifierEnumerator::IdentifierEnumerator(vector<Property*> & properties, 
            const IdentifierRegex& regexIdentifier, bool includeWhenMatches, const Property* requester) {
        mProperties = &properties;
        mPosition = 0;
        mIdentifierRegex = &regexIdentifier;
        mIncludeWhenMatches = includeWhenMatches;
        mRequester = requester;
    }

    IdentifierEnumerator::~IdentifierEnumerator() {
        delete mIdentifierRegex;
    }

    void IdentifierEnumerator::Reset() {
        Current = NULL;
        mPosition = 0;
    }

    bool IdentifierEnumerator::MoveNext() {
        while (mPosition<(int)mProperties->size()) {
            Current = (*mProperties)[mPosition++];
            if (Current != NULL) {
                if (mRequester == NULL || Current->InScope(mRequester, false)) {
                    if (mIdentifierRegex->Match(*Current->Identity)) {
                        if (mIncludeWhenMatches) {
                            return true;
                        }
                    } else {
                        if (!mIncludeWhenMatches) {
                            return true;
                        }
                    }
                }
            }
        }
        Current = NULL;
        return false;
    }
}
