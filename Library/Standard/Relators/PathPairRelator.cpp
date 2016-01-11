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
#include "Relator.h"
#include "Connector.h"
#include "ConnectorRelator.h"
#include "PathPairRelator.h"
#include "PathRegex.h"

namespace Plato {

    PathPairRelator::PathPairRelator(Container& parent, const Property* requester, Path& inputRegexPath, Path& outputRegexPath)
        : ConnectorRelator(parent) {
        Requester = requester;
        InputRegexPath = &inputRegexPath;
        OutputRegexPath = &outputRegexPath;
    }

    bool PathPairRelator::MatchAndValidRelation(Connector& connector, Property& input, Property& output) {
        if (PathRegex::CompareToRegex(&input, InputRegexPath, Requester, PropertyModes::Traversing)) {
            return PathRegex::CompareToRegex(&output, OutputRegexPath, Requester, PropertyModes::Traversing);
        }
        return false;
    }
}
