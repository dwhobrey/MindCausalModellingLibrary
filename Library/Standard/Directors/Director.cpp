#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "Director.h"

namespace Plato {

    const string* Director::InputName = NULL;
    const string* Director::OutputName = NULL;
    const string* Director::DefaultName = NULL;
    const string* Director::InputDirectorName = NULL;
    const string* Director::OutputDirectorName = NULL;
    const string* Director::DefaultDirectorName = NULL;

    void Director::Initializer() {
        InputName = new string("input");
        OutputName = new string("output");
        DefaultName = new string("default");
        InputDirectorName = new string("InputDirector");
        OutputDirectorName = new string("OutputDirector");
        DefaultDirectorName = new string("DefaultDirector");
    }

    void Director::Finalizer() {
        delete InputName;
        delete OutputName;
        delete DefaultName;
        delete InputDirectorName;
        delete OutputDirectorName;
        delete DefaultDirectorName;
    }

    Director::Director(Container* creator, Container* parent, Identifier& directorName, PropertyScopesEnum scope) 
        : Property(creator, parent, directorName, scope) {
            Flags.IsDirector = true;
    }

    Property* Director::Route(const Identifier* identifier, const Property* source, const Property* requester, PropertyModesEnum mode) {
        return NULL;
    }
}
