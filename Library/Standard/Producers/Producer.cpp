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

namespace Plato {

    const ClassTypeInfo* Producer::TypeInfo = NULL;

    void Producer::Initializer() {
        TypeInfo = new ClassTypeInfo("Producer",221,1,NULL,Property::TypeInfo);
    }

    void Producer::Finalizer() {
        delete TypeInfo;
    }

    Producer::Producer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionModes,
            IdentifierRegex& regexIdentifier, ProducerModesEnum producerMode, Generator* generator)
            : Property(NULL, NULL, name, scope) {
        RegexIdentifier = &regexIdentifier;
        ResolutionMode = resolutionModes;
        ProducerMode = producerMode;
        ProducerGenerator = generator;
        Flags.IsProducer = true;
    }

    Producer::~Producer() {
    }

    Property* Producer::Create(Container& container, const Identifier& identifier, const Property* requester) {
        // Call generator and Add to container.
        if (ProducerGenerator != NULL) {
            if(container.InScope(requester,false)) {
                Property* p = ProducerGenerator->GetProperty(container, identifier);
                if (p != NULL) {
                    container.Add(*p);
                    return p;
                }
            }
        }
        return NULL;
    }

    void Producer::Populate(Container& container, const IdentifierRegex& regexIdentifier, const Property* requester) {
        // Algorithm:
        // 1) Check if Unbounded -> get size from corresponding group.
        // 2) Efficiently check if properties already exist.
        //    Use producer as an enumerator for fixed identifier's. (Maps regexIdent to set of fixed).
        // 3) Check if property with identifier exists in container.
        // 4) Add if doesn't, else move to next.
        const PointRegex* position = regexIdentifier.Position;
        if (position != NULL 
                // && position->Status==PointStatus::Unbounded // DEBUG: Fix.
            ) {
            Connector* currentConnector = Model::Current().CurrentConnector;
            int count = currentConnector==NULL ? 0 : currentConnector->GetAppendCount(container, regexIdentifier);
            count = count;
            // DEBUG: TO DO: Provide default generic implementation.
        }
    }
}
