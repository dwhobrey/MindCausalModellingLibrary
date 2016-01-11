#include "PlatoIncludes.h"
#include "Regexes.h"
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
#include "Restrictor.h"
#include "Path.h"
#include "PathRestrictor.h"
#include "LocalRestrictor.h"
#include "PathOperators.h"

namespace Plato {

    const ClassTypeInfo* LocalRestrictor::TypeInfo = NULL;
    const LocalRestrictor* LocalRestrictor::LocalConnectorRestrictor = NULL;
    const LocalRestrictor* LocalRestrictor::LocalConverterRestrictor = NULL;

    void LocalRestrictor::Initializer() {
        TypeInfo = new ClassTypeInfo("LocalRestrictor",231,1,NULL,PathRestrictor::TypeInfo);
        LocalConnectorRestrictor = new LocalRestrictor(*new IdentifierRegex(NULL,NULL,new string("Connector")));
        LocalConverterRestrictor = new LocalRestrictor(*new IdentifierRegex(NULL,NULL,new string("Converter")));
    }

    void LocalRestrictor::Finalizer() {
        delete TypeInfo;
        delete LocalConnectorRestrictor;
        delete LocalConverterRestrictor;
    }

    LocalRestrictor::LocalRestrictor(IdentifierRegex& operatorRegexTypeName) 
            : PathRestrictor(operatorRegexTypeName, true, PropertyScopes::Public, 
                    *new Path(NULL,*Path::Relative / *Regexes::WildPatternString)) {
    }

    LocalRestrictor::~LocalRestrictor() {
    }
}
