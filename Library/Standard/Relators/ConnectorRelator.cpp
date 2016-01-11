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
#include "Relator.h"
#include "Connector.h"
#include "ConnectorRelator.h"

namespace Plato {

    ConnectorRelator::ConnectorRelator(Container& parent) {
        IsSymmetric = false;
        Parent = &parent;
    }

    bool ConnectorRelator::MatchAndValidRelation(Connector& connector, GroupContainer& inputs, GroupContainer& outputs) {
        // DEBUG: To Do. Test whether i/o elements match patterns.
        // Then check that specific relation holds.
        return false;
    }

    bool ConnectorRelator::MatchAndValidRelation(Connector& connector, Property& input, Property& output) {
        // DEBUG: To Do. Test whether i/o elements match patterns.
        // Then check that specific relation holds.
        return false;
    }

    int ConnectorRelator::GetInputArity() {
        return 1;
    }

    int ConnectorRelator::GetOutputArity() {
        return 1;
    }
}
