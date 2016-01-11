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
#include "Region.h"

#include "Alias.h"
#include "Connector.h"
#include "Converter.h"
#include "Disposition.h"
#include "Distributor.h"
#include "Link.h"
#include "Flow.h"
#include "Phenomenon.h"
#include "Restrictor.h"
#include "Translator.h"
#include "Transducer.h"

namespace Plato {

    const ClassTypeInfo* Region::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Region::PropertyTypes = NULL;

    void Region::Initializer() {
        TypeInfo = new ClassTypeInfo("Region",307,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(
            Region::TypeInfo, Alias::TypeInfo, Connector::TypeInfo, Converter::TypeInfo, 
            Disposition::TypeInfo, Distributor::TypeInfo, Flow::TypeInfo, Link::TypeInfo, 
            Phenomenon::TypeInfo, Restrictor::TypeInfo, Translator::TypeInfo,
            NULL);
    }

    void Region::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Region::Region(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, creator, parent, elementName, scope) {
    }

    Region::Region(Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, parent, parent, elementName, scope) {
    }

    Region::Region(Container* parent, const char* elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, parent, parent, *new Identifier(elementName), scope) {
    }

    Region::~Region() {
    }

    void Region::Update() {
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

    void Region::BirthDeath() {
    }

    void Region::Superposition() {
    }
}
