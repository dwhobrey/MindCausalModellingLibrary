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
#include "ThreadLocalVariable.h"
#include "Model.h"
#include "Pad.h"
#include "InputPad.h"
#include "OutputPad.h"
#include "Phenomenon.h"

#include "Alias.h"
#include "Connector.h"
#include "Converter.h"
#include "Disposition.h"
#include "Distributor.h"
#include "Link.h"
#include "Flow.h"
#include "Region.h"
#include "Restrictor.h"
#include "Translator.h"
#include "Transducer.h"

namespace Plato {

    const ClassTypeInfo* Phenomenon::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Phenomenon::PropertyTypes = NULL;

    void Phenomenon::Initializer() {
        TypeInfo = new ClassTypeInfo("Phenomenon",306,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(
            Phenomenon::TypeInfo, Alias::TypeInfo, Connector::TypeInfo, 
            Converter::TypeInfo, Disposition::TypeInfo, Distributor::TypeInfo, 
            Flow::TypeInfo, Link::TypeInfo, Phenomenon::TypeInfo, 
            Region::TypeInfo, Restrictor::TypeInfo, Translator::TypeInfo,
            NULL);
    }

    void Phenomenon::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Phenomenon::Phenomenon(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, creator, parent, elementName, scope) {
    }

    Phenomenon::Phenomenon(Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, parent, parent, elementName, scope) {
    }

    Phenomenon::Phenomenon(Container* parent, const char* elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, parent, parent, *new Identifier(elementName), scope) {
    }

    Phenomenon::~Phenomenon() {
    }

    void Phenomenon::Update() {
        // Update sub regions.
        vector<Property*>* properties;
        properties = GetCategory(*Region::TypeInfo);
        if(properties!=NULL) {
            Model::Update(*properties);
        }
        // Update DIDs.
        properties = GetCategory(*Disposition::TypeInfo);
        if (properties!=NULL) {
            Model::Update(*properties);
        }
        // DEBUG: To do: Update other kinds of elements.
    }

    void Phenomenon::BirthDeath() {
    }

    void Phenomenon::Superposition() {
    }
}
