#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Identifier.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Path.h"
#include "Distributor.h"
#include "Generator.h"

namespace Plato {

    const ClassTypeInfo* Distributor::TypeInfo = NULL;

    void Distributor::Initializer() {
        TypeInfo = new ClassTypeInfo("Distributor",203,1,NULL,Property::TypeInfo);
    }

    void Distributor::Finalizer() {
        delete TypeInfo;
    }

    Distributor::Distributor(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope, 
            Generator& generator, Path& regionPath, Translator& translator, Distribution& distribution) 
            : Property(creator,parent,elementName,scope) {
        Flags.ClassKind = ClassKinds::Distributor;
        // DEBUG: TO DO.
    }

    Distributor::Distributor(Generator& generator, Path& regionPath, Translator& translator, Distribution& distribution) 
        : Property(NULL, NULL, Identifier::Generate("Distributor"), PropertyScopes::Private) {
        Flags.ClassKind = ClassKinds::Distributor;
        // DEBUG: TO DO.
    }

    Distributor::Distributor(Generator& generator, Path& regionPath) 
        : Property(NULL, NULL, Identifier::Generate("Distributor"), PropertyScopes::Private) {
        Flags.ClassKind = ClassKinds::Distributor;
        // DEBUG: TO DO.
    }

    void Distributor::Apply(ResolutionModesEnum resolutionMode, Container& parent) {
        // DEBUG: TO DO.
    }
}
