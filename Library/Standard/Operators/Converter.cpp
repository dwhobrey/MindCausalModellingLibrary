#include "PlatoIncludes.h"
#include "Strings.h"
#include "Numbers.h"
#include "ClassTypeInfo.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Identifier.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Path.h"
#include "Point.h"
#include "Relator.h"
#include "ProximityRelator.h"
#include "Converter.h"

#include "Alias.h"
#include "Connector.h"
#include "Disposition.h"
#include "Distributor.h"
#include "Flow.h"
#include "Link.h"
#include "Phenomenon.h"
#include "Region.h"
#include "Restrictor.h"
#include "Translator.h"
#include "ConverterRestrictor.h"

namespace Plato {

    const ClassTypeInfo* Converter::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Converter::PropertyTypes = NULL;

    void Converter::Initializer() {
        TypeInfo = new ClassTypeInfo("Converter",202,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(
            Alias::TypeInfo, Connector::TypeInfo, Converter::TypeInfo, Disposition::TypeInfo, Distributor::TypeInfo, 
            Flow::TypeInfo, Link::TypeInfo, Phenomenon::TypeInfo, Region::TypeInfo, Restrictor::TypeInfo, Translator::TypeInfo,
            NULL);
    }

    void Converter::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Converter::Converter(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionMode, ProximityRelator& proximityRelator) 
            : Container(*PropertyTypes,NULL, NULL, name, scope) {
        Flags.ClassKind = ClassKinds::Converter;
        ResolutionMode = resolutionMode;
        ElementRelator = &proximityRelator;
    }

    void Converter::Apply(ResolutionModesEnum resolutionMode, Container& parent, vector<Property*> & elements) {
        // Conversion is up to derived class.
    }

    void Converter::Apply(ResolutionModesEnum resolutionMode, Container& parent) {
        if (ElementRelator != NULL) {
            vector<vector<Property*>*>* tuples = ElementRelator->FindMatches(parent);
            if (tuples != NULL) {
                vector<vector<Property*>*>::iterator itr;
                for(itr=tuples->begin();itr!=tuples->end();++itr) {
                    vector<Property*>* elements = *itr;
                    if (parent.IsUnrestricted || ConverterRestrictor::IsAllowed(ResolutionMode, parent, *elements)) {
                        Apply(resolutionMode, parent, *elements);
                    }
                    delete elements;
                }
                delete tuples;
            }
        }
    }
}
