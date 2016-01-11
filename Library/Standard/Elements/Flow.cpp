#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Pad.h"
#include "InputPad.h"
#include "OutputPad.h"
#include "Flow.h"

#include "Alias.h"
#include "Connector.h"
#include "Converter.h"
#include "Disposition.h"
#include "Distributor.h"
#include "Link.h"
#include "Phenomenon.h"
#include "Region.h"
#include "Restrictor.h"
#include "Translator.h"
#include "Transducer.h"

namespace Plato {

    const ClassTypeInfo* Flow::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Flow::PropertyTypes = NULL;

    void Flow::Initializer() {
        TypeInfo = new ClassTypeInfo("Flow",305,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(
            Flow::TypeInfo, Alias::TypeInfo, Connector::TypeInfo, Converter::TypeInfo, 
            Disposition::TypeInfo, Distributor::TypeInfo, Link::TypeInfo, 
            Translator::TypeInfo, Transducer::TypeInfo,
            NULL);
    }

    void Flow::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Flow::Flow(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, creator, parent, elementName, scope) {
    }

    Flow::~Flow() {
    }

    void Flow::Update() {
    }

    void Flow::BirthDeath() {
    }

    void Flow::Superposition() {
    }
}
