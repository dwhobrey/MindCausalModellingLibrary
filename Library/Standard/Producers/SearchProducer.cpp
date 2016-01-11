#include "PlatoIncludes.h"
#include "Regexes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Property.h"
#include "ThreadLocalVariable.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "ProducerModes.h"
#include "Container.h"
#include "Producer.h"
#include "Generator.h"
#include "IdentifierRegex.h"
#include "Point.h"
#include "PointSpace.h"
#include "PointRegex.h"
#include "Connector.h"
#include "Model.h"
#include "SearchProducer.h"

namespace Plato {

    const ClassTypeInfo* SearchProducer::TypeInfo = NULL;

    void SearchProducer::Initializer() {
        TypeInfo = new ClassTypeInfo("SearchProducer",222,1,NULL,Producer::TypeInfo);
    }

    void SearchProducer::Finalizer() {
        delete TypeInfo;
    }

    SearchProducer::SearchProducer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionModes, 
                                   IdentifierRegex& regexPattern, ProducerModesEnum producerMode) 
        : Producer(name, scope, resolutionModes, regexPattern, producerMode, NULL) {
    }

    Property* SearchProducer::Create(Container& container, Identifier& identifier, const Property* requester) {
        // Find a Producer.
        Container* searchContainer = container.Parent;
        while (searchContainer != NULL) {
            vector<Property*>* properties = searchContainer->GetCategory(*Producer::TypeInfo);
            if (properties != NULL) {
                vector<Property*>::iterator itr;
                for(itr=properties->begin();itr!=properties->end();++itr) {
                    Property* prop = (*itr)->GetValue();
                    if(prop->Flags.IsProducer) {
                        Producer* p = (Producer*)prop;
                        if (p->RegexIdentifier->Match(identifier)) {
                            prop = p->Create(container, identifier, requester);
                            if (prop != NULL) {
                                return prop;
                            }
                        }
                    }
                }
            }
            searchContainer = searchContainer->Parent;
        }
        return NULL;
    }

    void SearchProducer::Populate(Container& container, IdentifierRegex& regexIdentifier, const Property* requester) {
        // Algorithm:
        // 1) Check if Unbounded -> get size from corresponding group.
        // 2) Efficiently check if properties already exist.
        //    Use producer as an enumerator for fixed identifier's. (Maps regexIdent to set of fixed).
        // 3) Check if property with identifier exists in container.
        // 4) Add if doesn't, else move to next.
        const PointRegex* position = regexIdentifier.Position;
        if (position != NULL && position->Status==PointRegexStatus::Unbounded) {
            Connector* currentConnector = Model::Current().CurrentConnector;
            int count = (currentConnector == NULL) ? 0 : currentConnector->GetAppendCount(container, regexIdentifier);
            count = count;
            // DEBUG: TO DO: Provide default generic implementation.
        }
    }
}
