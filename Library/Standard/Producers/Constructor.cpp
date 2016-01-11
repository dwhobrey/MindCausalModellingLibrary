#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "Constructor.h"

namespace Plato {

    const ClassTypeInfo* Constructor::TypeInfo = NULL;

    void Constructor::Initializer() {
        TypeInfo = new ClassTypeInfo("Constructor",220,1,NULL,Property::TypeInfo);
    }

    void Constructor::Finalizer() {
        delete TypeInfo;
    }

    Constructor::Constructor(Container* creator, Container* parent, Identifier& constructorName,
            PropertyScopesEnum constructorScope, 
            const ClassTypeInfo& propertyClassTypeInfo, Arguments* arguments) 
        : Property(creator,parent,constructorName,constructorScope) {
        PropertyClassTypeInfo = &propertyClassTypeInfo;
        PropertyArguments = arguments;
        Flags.IsConstructor = true;
    }

    Constructor::~Constructor() {
    }

    Property* Constructor::Factory(Arguments* arguments) {
        return (*(PropertyClassTypeInfo->Factory))(arguments==NULL ? PropertyArguments : arguments);
    }
}
