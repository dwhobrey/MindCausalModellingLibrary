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
#include "Transducer.h"
#include "Relator.h"
#include "ConnectorRelator.h"
#include "Pad.h"
#include "InputPad.h"
#include "OutputPad.h"

#include "Alias.h"
#include "Connector.h"
#include "Converter.h"
#include "Disposition.h"
#include "Distributor.h"
#include "Flow.h"
#include "Link.h"
#include "Phenomenon.h"
#include "Region.h"
#include "Restrictor.h"
#include "Translator.h"

namespace Plato {

    const ClassTypeInfo* Transducer::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Transducer::PropertyTypes = NULL;

    void Transducer::Initializer() {
        TypeInfo = new ClassTypeInfo("Transducer",241,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(
            Transducer::TypeInfo, Alias::TypeInfo, Connector::TypeInfo, Converter::TypeInfo, 
            Disposition::TypeInfo, Distributor::TypeInfo, Flow::TypeInfo, Link::TypeInfo, 
            Phenomenon::TypeInfo, Region::TypeInfo, Restrictor::TypeInfo, Translator::TypeInfo,
            NULL);
    }

    void Transducer::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Transducer::Transducer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionMode, ConnectorRelator& elementRelator) 
        : Container(*PropertyTypes,NULL, NULL, name, scope) {
        ResolutionMode = resolutionMode;
        ElementRelator = &elementRelator;
        Flags.IsTransducer = true;
    }

    Transducer::~Transducer() {
        delete ElementRelator;
    }

    Transducer* Transducer::Factory(ResolutionModesEnum resolutionMode, Container& parent, const Property* requester, 
            Connector& connector, InputPad& toPad, OutputPad& fromPad) {
        if (Matches(resolutionMode, parent, connector, toPad, fromPad)) {
            return this;
        }
        return NULL;
    }

    void Transducer::GroupApply(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector, 
            GroupContainer& inputs, GroupContainer& outputs) {
    }

    void Transducer::Apply(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector, 
        Property& input, Property& output) {
    }

    bool Transducer::GroupMatches(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector, 
            GroupContainer& inputs, GroupContainer& outputs) {
        return ElementRelator->MatchAndValidRelation(connector, inputs, outputs);
    }

    bool Transducer::Matches(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector, 
            Property& input, Property& output) {
        return ElementRelator->MatchAndValidRelation(connector, input, output);
    }
}
