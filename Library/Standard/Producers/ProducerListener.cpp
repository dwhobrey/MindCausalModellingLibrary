#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "Container.h"
#include "ResolutionModes.h"
#include "ProducerModes.h"
#include "ProducerListener.h"
#include "Producer.h"

namespace Plato {

    ProducerListener::ProducerListener(Container& container) {
        mContainer = &container;
    }

    Property* ProducerListener::Create(const Identifier& identifier, const Property* requester) {
        vector<Property*>* properties = mContainer->GetCategory(*Producer::TypeInfo);
        if (properties != NULL) {
            vector<Property*>::size_type index = 0;
            while(index<properties->size()) {
                Producer* p = (Producer*)((*properties)[index++]->GetValue());
                if (p != Property::NullProperty && p->RegexIdentifier->Match(identifier)) {
                    Property* prop = p->Create(*mContainer, identifier, requester);
                    if (prop != NULL) {
                        return prop;
                    }
                }
            }
        }
        return NULL;
    }

    void ProducerListener::Populate(const IdentifierRegex& regexIdentifier, const Property* requester) {
        // First check if there are any producers in container for identifier.
        // Need to do a two way regex match: i.e. producer name may be a regex too.
        // Call each producer in reverse order.
        vector<Property*>* properties = mContainer->GetCategory(*Producer::TypeInfo);
        if (properties != NULL) {
            vector<Property*>::size_type index = 0;
            while(index<properties->size()) {
                Producer* p = (Producer*)((*properties)[index++]->GetValue());
                if (p != Property::NullProperty && p->RegexIdentifier->PatternMatch(regexIdentifier)) {
                    p->Populate(*mContainer, regexIdentifier, requester);
                }
            }
        }
    }
}
