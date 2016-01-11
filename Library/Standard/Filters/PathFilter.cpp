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
#include "PathFilter.h"
#include "PathRegex.h"
#include "Path.h"
#include "ScopeEnumerator.h"

namespace Plato {

    PathFilterArguments::PathFilterArguments(Path& regexPath, bool includeWhenMatches) {
        IncludeWhenMatches = includeWhenMatches;
        RegexPath = &regexPath;
    }
    PathFilterArguments::PathFilterArguments(const PathFilterArguments& arguments) {
        IncludeWhenMatches = arguments.IncludeWhenMatches;
        RegexPath = new Path(*(arguments.RegexPath));
    }

    PathFilterArguments::~PathFilterArguments() {
        delete RegexPath;
    }
    Arguments* PathFilterArguments::Clone() const {
        return new PathFilterArguments(*this);
    }

    const ClassTypeInfo* PathFilter::TypeInfo = NULL;

    void PathFilter::Initializer() {
        TypeInfo = new ClassTypeInfo("PathFilter",105,1,(ClassTypeInfo::FactoryDelegate)PathFilter::Factory,Filter::TypeInfo);
    }

    void PathFilter::Finalizer() {
        delete TypeInfo;
    }

    PathFilter::PathFilter(FilterArguments& arguments) 
        : Filter(*new Identifier("PathFilter",NULL,NULL),arguments) {
    }

    PathFilter::~PathFilter() {
    }

    void PathFilter::RefreshContents() {
        Flags.Refresh = false;
        PathFilterArguments* pfa = (PathFilterArguments*)(Arguments->AdditionalArguments);
        Path* regexPath = pfa->RegexPath;
        ScopeEnumerator* e = new ScopeEnumerator(*(Arguments->Source),Arguments->Requester);
        if(pfa->IncludeWhenMatches) {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if(PathRegex::CompareToRegex(p, regexPath, Arguments->Requester, Arguments->Mode)) {
                    Add(*p);
                }
            }
        } else {
            while (e->MoveNext()) {
                Property* p = e->Current;
                if(PathRegex::CompareToRegex(p, regexPath, Arguments->Requester, Arguments->Mode)) {
                    Add(*p);
                }
            }
        }
        delete e;
    }

    PathFilter* PathFilter::Factory(FilterArguments* arguments) {
        return new PathFilter(*arguments);
    }
}
