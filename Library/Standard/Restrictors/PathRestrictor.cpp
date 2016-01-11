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
#include "Restrictor.h"
#include "Path.h"
#include "PathRestrictor.h"

namespace Plato {

    const ClassTypeInfo* PathRestrictor::TypeInfo = NULL;

    void PathRestrictor::Initializer() {
        TypeInfo = new ClassTypeInfo("PathRestrictor",232,1,NULL,Restrictor::TypeInfo);
    }

    void PathRestrictor::Finalizer() {
        delete TypeInfo;
    }

    PathRestrictor::PathRestrictor(IdentifierRegex& operatorRegexTypeName, bool includeWhenMatches, PropertyScopesEnum scope, Path& path)
        : Restrictor(operatorRegexTypeName, includeWhenMatches, scope) {
        RestrictionPath = &path;
    }

    PathRestrictor::~PathRestrictor() {
        delete RestrictionPath;
    }
}
