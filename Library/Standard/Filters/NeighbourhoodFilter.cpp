#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "ClassCatalogue.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "Container.h"
#include "Arguments.h"
#include "Filter.h"
#include "NeighbourhoodFilter.h"

namespace Plato {

    NeighbourhoodFilterArguments::NeighbourhoodFilterArguments(int numberOfLayers) {
        IsLayers = true;
        NumberOfLayers = numberOfLayers;
    }

    NeighbourhoodFilterArguments::NeighbourhoodFilterArguments(float percentageDistance) {
        IsLayers = false;
        PercentageDistance = percentageDistance;
    }
    NeighbourhoodFilterArguments::NeighbourhoodFilterArguments(const NeighbourhoodFilterArguments& arguments) {
        IsLayers = arguments.IsLayers;
        if(IsLayers) {
            NumberOfLayers = arguments.NumberOfLayers;
        } else {
            PercentageDistance = arguments.PercentageDistance;
        }
    }
    Arguments* NeighbourhoodFilterArguments::Clone() const {
        return new NeighbourhoodFilterArguments(*this);
    }

    const ClassTypeInfo* NeighbourhoodFilter::TypeInfo = NULL;

    void NeighbourhoodFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("NeighbourhoodFilter",104,1,(ClassTypeInfo::FactoryDelegate)NeighbourhoodFilter::Factory,Filter::TypeInfo);
    }

    void NeighbourhoodFilter::Finalizer() {
        delete TypeInfo;
    }

    NeighbourhoodFilter::NeighbourhoodFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("NeighbourhoodFilter",NULL,NULL),arguments) {
    }

    void NeighbourhoodFilter::RefreshContents() {
        Flags.Refresh = false;
    }

    NeighbourhoodFilter* NeighbourhoodFilter::Factory(FilterArguments* arguments) {
        return new NeighbourhoodFilter(*arguments);
    }
}
